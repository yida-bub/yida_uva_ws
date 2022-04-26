#include <ros/ros.h>
#include <stdio.h>
#include <math.h>
#include <geometry_msgs/PoseStamped.h>  //发布的消息体对应的头文件，该消息体的类型为geometry_msgs：：PoseStamped
#include <mavros_msgs/CommandBool.h>  //CommandBool服务的头文件，该服务的类型为mavros_msgs：：CommandBool
#include <mavros_msgs/SetMode.h>     //SetMode服务的头文件，该服务的类型为mavros_msgs：：SetMode
#include <mavros_msgs/State.h>  //订阅的消息体的头文件，该消息体的类型为mavros_msgs：：State
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/RCIn.h>
 
#define TAKOFF_HIGH 3

enum STEP{
    PREPARE, //准备
    TAKOFF, //起任务
    TASK_FIRST, //任务1
    TASK_SECOND, //任务2
    TASK_THIRD, //任务3
    TASK_FOURTH, //任务4
    TASK_FIFTH, //任务5
    TASK_SIXTH, //任务6
}step_flow;

//任务运行状态
bool step_state = true;
//建立一个订阅消息体类型的变量，用于存储订阅的信息
mavros_msgs::State current_state; //订阅消息体类型的变量
geometry_msgs::PoseStamped high; //订阅，主要获取无人机的z轴高度信息

mavros_msgs::PositionTarget velocity_msg; //消息实例，
geometry_msgs::PoseStamped pose;
mavros_msgs::RCIn rcin_channel;

//订阅时的回调函数，接受到该消息体的内容时执行里面的内容，这里面的内容就是赋值
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}
void high_fun(const geometry_msgs::PoseStamped::ConstPtr& msg){
    high = *msg;
}
void rcin_fun(const mavros_msgs::RCIn::ConstPtr &msg){
	rcin_channel = *msg;
	if(rcin_channel.channels[6-1] >= 1500){
		step_state = false;
        // printf("step_state false");
	}else{
        // printf("step_state true");
		step_state = true;
	}
}
void pose_control(double const x, double const y, double const z){
    pose.pose.position.x = x;
    pose.pose.position.y = y;
    pose.pose.position.z = z;
}
// 机体 - xyz速度，偏航速度
void position_control_body_vxyzyawr(double const vx, double const vy, double const vz, float const yawr)
{
	velocity_msg.coordinate_frame = mavros_msgs::PositionTarget::FRAME_BODY_NED;
	velocity_msg.type_mask = 0b011111000111;
	velocity_msg.velocity.x = vx;
	velocity_msg.velocity.y = vy;
	velocity_msg.velocity.z = vz;
    velocity_msg.yaw_rate = yawr;
	velocity_msg.header.stamp = ros::Time::now();
}
// body - xyz位置，偏航度
void position_control_bady_xyzyaw(double const x, double const y, double const z, float const yaw){
	velocity_msg.coordinate_frame = mavros_msgs::PositionTarget::FRAME_BODY_NED;
	velocity_msg.type_mask = 0b101111111000;
	velocity_msg.position.x = x;
	velocity_msg.position.y = y;
	velocity_msg.position.z = z;
	velocity_msg.yaw = yaw;
	velocity_msg.header.stamp = ros::Time::now();
}
// home - xyz位置，偏航度
void position_control_local_xyzyaw(double const x, double const y, double const z, float const yaw){
	velocity_msg.coordinate_frame = mavros_msgs::PositionTarget::FRAME_LOCAL_NED;
	velocity_msg.type_mask = 0b101111111000;
	velocity_msg.position.x = x;
	velocity_msg.position.y = y;
	velocity_msg.position.z = z;
	velocity_msg.yaw = yaw;
	velocity_msg.header.stamp = ros::Time::now();
}
// home - xyz位置，偏航sudu
void position_control_local_xyzyawr(double const x, double const y, double const z, float const yawr){
	velocity_msg.coordinate_frame = mavros_msgs::PositionTarget::FRAME_LOCAL_NED;
	velocity_msg.type_mask = 0b011111111000;
	velocity_msg.position.x = x;
	velocity_msg.position.y = y;
	velocity_msg.position.z = z;
	velocity_msg.yaw_rate = yawr;
	velocity_msg.header.stamp = ros::Time::now();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node"); //ros系统的初始化，最后一个参数为节点名称
    ros::NodeHandle nh;
 
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");//启动服务1
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");//启动服务2

    ros::Publisher local_position_pub = nh.advertise<mavros_msgs::PositionTarget>("mavros/setpoint_raw/local", 10);
    ros::Subscriber high_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, high_fun);
	ros::Subscriber local_rc_in = nh.subscribe<mavros_msgs::RCIn>("/mavros/rc/in", 10, rcin_fun);
    
    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);


    // current_state
    // 等待飞控连接mavros，current_state是我们订阅的mavros的状态，连接成功在跳出循环
    while(ros::ok() && !current_state.connected){
        ROS_INFO("connected......");
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("connected_ok!");
 
    //先实例化一个geometry_msgs::PoseStamped类型的对象，并对其赋值，最后将其发布出去
    // pose_control(0,0,TAKOFF);
    // local_pos_pub.publish(pose); 

    //建立一个类型为SetMode的服务端offb_set_mode，并将其中的模式mode设为"OFFBOARD"，作用便是用于后面的
    //客户端与服务端之间的通信（服务）
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";
 
    //建立一个类型为CommandBool的服务端arm_cmd，并将其中的是否解锁设为"true"，作用便是用于后面的
    //客户端与服务端之间的通信（服务）
    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;
 
    //起飞点高度记录
    double home_high = high.pose.position.z;
    ROS_INFO("home_high = %lf", home_high);
    //更新时间
    ros::Time last_request = ros::Time::now();
    ros::Time step_time = ros::Time::now();
    
    int uva_task_stat = TAKOFF;

    while(ros::ok())//进入大循环
    {
        //首先判断当前模式是否为offboard模式，如果不是，则客户端set_mode_client向服务端offb_set_mode发起请求call，
        //然后服务端回应response将模式返回，这就打开了offboard模式
        if( current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)
            {
                ROS_INFO("Offboard enabled");//打开模式后打印信息
            }
            // uva_task_stat = TAKOFF;
            last_request = ros::Time::now();
            step_time = ros::Time::now();
        }
        else //else指已经为offboard模式，然后进去判断是否解锁，如果没有解锁，则客户端arming_client向服务端arm_cmd发起请求call
            //然后服务端回应response成功解锁，这就解锁了
        {
            if( !current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) && arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");//解锁后打印信息
                }
                last_request = ros::Time::now();
                step_time = ros::Time::now();
                uva_task_stat = TAKOFF;
            }
        }
        if (!step_state) //6通道打开，退出程序
        {
            printf("Program EXIT !!!");
            return 0;
        }
        //保持高度
        if((fabs(high.pose.position.z - home_high) <= TAKOFF_HIGH-0.5 || fabs(high.pose.position.z - home_high) >= TAKOFF_HIGH+0.5) && uva_task_stat != TAKOFF){
            position_control_local_xyzyawr(high.pose.position.x,high.pose.position.y,TAKOFF_HIGH+home_high,0);
            local_position_pub.publish(velocity_msg);
            // printf("high!!!\n");
            ros::spinOnce();
            rate.sleep();
            continue;
        }
        /*
        任务流程
        1.起飞，机体坐标起飞，z轴速度，定偏航
        2.x方向速度控制移动，定偏航
        3.y方向速度控制移动，定偏航
        4.y方向、偏航速度控制，实现转圈
        5.尝试在执行任务的时候保持无人机的高度
        6.实际飞行中看看在没有控制高度的情况下，飞行的情况
        7.
        */
        switch(uva_task_stat){
            case TAKOFF: //起飞
                // position_control_local_zyaw(3,0);
                position_control_local_xyzyawr(0,0,TAKOFF_HIGH,0);
                local_position_pub.publish(velocity_msg);
                // pose_control(0,0,TAKOFF_HIGH);
                // local_pos_pub.publish(pose); 

                // ROS_INFO("%lf\t%lf\n", high.pose.position.z, home_high);
                // if(ros::Time::now() - step_time >= ros::Duration(10.0)){
                if(fabs(high.pose.position.z - home_high) >= TAKOFF_HIGH*0.9){
                    uva_task_stat = TASK_FIRST;
                    step_time = ros::Time::now();
                    ROS_INFO("GO_TASK_FIRST !!!");
                }
                break;
            case TASK_FIRST: //x方向速度运动
                position_control_body_vxyzyawr(1,0,0,0);
                local_position_pub.publish(velocity_msg);
                if(ros::Time::now() - step_time >= ros::Duration(8.0)){
                    uva_task_stat = TASK_SECOND;
                    step_time = ros::Time::now();
                    ROS_INFO("GO_TASK_SECOND !!!");
                }
                break;
            case TASK_SECOND: //x方向向左走圆弧
                position_control_body_vxyzyawr(1,0,0,0.5);
                local_position_pub.publish(velocity_msg);
                if(ros::Time::now() - step_time >= ros::Duration(15.0)){
                    uva_task_stat = TASK_THIRD;
                    step_time = ros::Time::now();
                    ROS_INFO("GO_TASK_THIRD !!!");
                }
                break;
            case TASK_THIRD: //x方向向右走圆弧
                position_control_body_vxyzyawr(1,0,0,-0.5);
                local_position_pub.publish(velocity_msg);
                if(ros::Time::now() - step_time >= ros::Duration(15.0)){
                    uva_task_stat = TASK_FIFTH;
                    step_time = ros::Time::now();
                    ROS_INFO("GO_TASK_FIFTH !!!");
                }
                break;
            case TASK_FIFTH: //返回home起飞点
                position_control_local_xyzyaw(0,0,TAKOFF_HIGH,0);
                local_position_pub.publish(pose);
                // if(ros::Time::now() - step_time >= ros::Duration(10.0)){
                //     uva_task_stat = TASK_SIXTH;
                //     step_time = ros::Time::now();
                //     ROS_INFO("GO_TASK_SIXTH !!!");
                // }
                break;
        }
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
