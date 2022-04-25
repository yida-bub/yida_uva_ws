#include "object_track/object_track.hpp"

enum STEP
{
	PREPARE,	 //准备
	TAKOFF,		 //起飞任务
	OBJECT_TASK, //目标检测跟踪任务
	TASK_FIRST,	 //任务1
	TASK_SECOND, //任务2
	TASK_THIRD,	 //任务3
	TASK_FOURTH, //任务4
	TASK_FIFTH,	 //任务5
	TASK_SIXTH,	 //任务6
} step_flow;

// ROS实例
mavros_msgs::State current_state;		  //订阅消息体类型的变量
geometry_msgs::PoseStamped high;		  //订阅，主要获取无人机的z轴高度信息
mavros_msgs::PositionTarget velocity_msg; //消息实例，
geometry_msgs::PoseStamped pose;
//建立一个类型为SetMode的服务端offb_set_mode，并将其中的模式mode设为"OFFBOARD"，作用便是用于后面的
//客户端与服务端之间的通信（服务）
mavros_msgs::SetMode offb_set_mode;
//建立一个类型为CommandBool的服务端arm_cmd，并将其中的是否解锁设为"true"，作用便是用于后面的
//客户端与服务端之间的通信（服务）
mavros_msgs::CommandBool arm_cmd;
mavros_msgs::RCIn rcin_channel;

//订阅时的回调函数，接受到该消息体的内容时执行里面的内容，这里面的内容就是赋值
void state_cb(const mavros_msgs::State::ConstPtr &msg)
{
	current_state = *msg;
}
void high_fun(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
	high = *msg;
}
void rcin_fun(const mavros_msgs::RCIn::ConstPtr &msg){
	rcin_channel = *msg;
}
void pose_control(double const x, double const y, double const z)
{
	pose.pose.position.x = x;
	pose.pose.position.y = y;
	pose.pose.position.z = z;
}
// 速度、偏航角控制
void position_control_xyz(double const z, double const vx, float const yawr)
{
	velocity_msg.coordinate_frame = mavros_msgs::PositionTarget::FRAME_BODY_NED;
	velocity_msg.type_mask = mavros_msgs::PositionTarget::IGNORE_PX | // Position ignore flags 位置
							 mavros_msgs::PositionTarget::IGNORE_PY |
							 //  mavros_msgs::PositionTarget::IGNORE_PZ |
							 // mavros_msgs::PositionTarget::IGNORE_VX | //Velocity vector ignore flags 速度
							 mavros_msgs::PositionTarget::IGNORE_VY |
							 mavros_msgs::PositionTarget::IGNORE_VZ |
							 mavros_msgs::PositionTarget::IGNORE_AFX | // Acceleration/Force vector ignore flags 加速度
							 mavros_msgs::PositionTarget::IGNORE_AFY |
							 mavros_msgs::PositionTarget::IGNORE_AFZ |
							 mavros_msgs::PositionTarget::FORCE |
							 // mavros_msgs::PositionTarget::IGNORE_YAW  //偏航
							 mavros_msgs::PositionTarget::IGNORE_YAW_RATE //偏航率
		;
	velocity_msg.velocity.x = vx;
	// velocity_msg.yaw = yaw;
	velocity_msg.yaw_rate = yawr;
	velocity_msg.position.z = z;
	velocity_msg.header.stamp = ros::Time::now();
}

int main(int argc, char **argv)
{
	//组织数据文件的路径，获取最新的labels文件
	char path[128] = YOLO_RUNS_DETECT_PATH;
	char exp_file_path[128] = {};
	getnewfile(path, exp_file_path);
	strcat(exp_file_path, "/labels/");
	// printf("%s\n", newfile_path);

	data_t *new_data = (data_t *)(malloc(sizeof(data_t)));
	data_t *old_data = (data_t *)(malloc(sizeof(data_t)));
	move_t *move = (move_t *)(malloc(sizeof(move_t)));

	// new_data的初始化，假设刚开始的目标点在图像中心
	new_data->classes = 0;
	new_data->x_point = 0.5;
	new_data->y_point = 0.5;
	new_data->x_len = 0.1;
	new_data->y_len = 0.2;
	*old_data = *new_data;

	// ROS节点初始化
	ros::init(argc, argv, "object_track_main");
	// 创建节点句柄
	ros::NodeHandle nh;

	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
	ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming"); //启动服务1
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");		//启动服务2
	ros::Publisher local_position_pub = nh.advertise<mavros_msgs::PositionTarget>("mavros/setpoint_raw/local", 10);
	ros::Subscriber high_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, high_fun);
	ros::Publisher local_rc_in = nh.advertise<mavros_msgs::RCIn>("/mavros/rc/in", 10, rcin_fun);
	// 设置循环的频率
	ros::Rate rate(20.0);

	// 等待飞控连接mavros，current_state是我们订阅的mavros的状态，连接成功在跳出循环
	while (ros::ok() && !current_state.connected)
	{
		ROS_INFO("connected......");
		ros::spinOnce();
		rate.sleep();
	}
	ROS_INFO("connected_ok!");
	//首次发布位置，起飞
	pose_control(0, 0, TAKOFF_HIGH);
	local_pos_pub.publish(pose);

	offb_set_mode.request.custom_mode = "OFFBOARD"; //模式切为 offboard
	arm_cmd.request.value = true;					//解锁
													//起飞点高度记录
	double home_high = high.pose.position.z;
	ROS_INFO("home_high = %lf", home_high);
	//更新时间
	ros::Time last_request = ros::Time::now();
	ros::Time step_time = ros::Time::now();

	int uva_task_stat = TAKOFF; //当前任务步骤

	while (ros::ok())
	{
		//首先判断当前模式是否为offboard模式，如果不是，则客户端set_mode_client向服务端offb_set_mode发起请求call，
		//然后服务端回应response将模式返回，这就打开了offboard模式
		if (current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0)))
		{
			if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)
			{
				ROS_INFO("Offboard enabled"); //打开模式后打印信息
			}
			last_request = ros::Time::now();
		}
		else // else指已经为offboard模式，然后进去判断是否解锁，如果没有解锁，则客户端arming_client向服务端arm_cmd发起请求call
			 //然后服务端回应response成功解锁，这就解锁了
		{
			if (!current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0)))
			{
				if (arming_client.call(arm_cmd) && arm_cmd.response.success)
				{
					ROS_INFO("Vehicle armed"); //解锁后打印信息
				}
				last_request = ros::Time::now();
				step_time = ros::Time::now();
				uva_task_stat = TAKOFF;
			}
		}
		switch (uva_task_stat)
		{
		case TAKOFF:
			pose_control(0, 0, TAKOFF_HIGH);
			local_pos_pub.publish(pose);
			if (fabs(high.pose.position.z - home_high) >= TAKOFF_HIGH * 0.9)
			{
				uva_task_stat = OBJECT_TASK;
				step_time = ros::Time::now();
				ROS_INFO("GO_OBJECT_TASK !!!");
			}
			break;
		case OBJECT_TASK:
			if (data_readtodisp(new_data, old_data, exp_file_path, move) == -1)
			{
				continue;
			}
			ROS_INFO("x_vel:%lf  y_vel:%lf  z_vel:%lf  yaw:%lf\n", move->x_vel, move->y_vel, move->z_vel, move->yawr);
			position_control_xyz(TAKOFF_HIGH, move->x_vel, move->yawr); //TAKOFF_HIGH 不行，需要发布不能以机体为坐标为点的位置
            local_position_pub.publish(velocity_msg); //尝试发布两个不同的local_position_pub
			
			if(rcin_channel.channels[5] >= 1250){
				//直接拿取遥控器的输入，来退出offboard模式
				//需要查一下它的消息结构！！！
				//不发布任何指令，可以直接return退出程序，转手动控制
			}
			break;
		}
		// 按照循环频率延时
		printf("\n");
		rate.sleep();
	}
	return 0;
}
