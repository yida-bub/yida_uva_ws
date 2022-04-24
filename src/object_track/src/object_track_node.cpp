#include "object_track/object_track.hpp"

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
	ros::NodeHandle n;
	// // 创建一个Publisher，发布名为/data_info的topic，消息类型为conrol_pkg::message_data，队列长度10
	// ros::Publisher data_info_pub = n.advertise<uva_control_topic::msg_data_struct>("/data_info", 10);
	// 设置循环的频率
	ros::Rate loop_rate(5.0);

	while (ros::ok())
	{
		if(data_readtodisp(new_data, old_data, exp_file_path, move) == 0){
			printf("x_vel:%lf  y_vel:%lf  z_vel:%lf  yaw:%lf\n", move->x_vel, move->y_vel, move->z_vel, move->yawr);
		}else{
			continue;
		}

		// 按照循环频率延时
		printf("\n");
		loop_rate.sleep();

		// sleep_ms(500);
		// sleep();
		// printf("*************************\n");
	}
	return 0;
}
