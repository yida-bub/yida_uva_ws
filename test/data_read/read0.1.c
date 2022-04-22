#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>	//文件读写
#include <string.h> //字符串操作

#define PI 3.1415926535898
#define DEC (PI / 180)
#define Y_OVERLOOK_K 0.12 //图像中心点y坐标无动作范围
#define X_OVERLOOK_K 0.20 //图像中心点x坐标无动作范围

//存放读取到的数据结构体
typedef struct dataest
{
	int classes;
	double x_point; //目标中心点相对左上角 x 坐标值
	double y_point; //目标中心点相对左上角 y 坐标值
	double x_len;	//目标的宽度
	double y_len;	//目标的高度
} data_t;
typedef struct moveest
{
	double x_vel; //无人机前进速度
	double y_vel;
	double z_vel; //对应无人机的高度，
	double yaw;	  //对应 data 的 x_point, 无人机航向角，及偏航
} move_t;
data_t getdata(char *const path, int const file_num, int const camere_num, data_t* const olddata)
{
	//组织数据文件的路径
	char data_path[256] = {};
	sprintf(data_path, "%s%hd_%hd.txt", path, camere_num, file_num);
	printf("data_path: %s\n", data_path);

	//数据的读取
	int fd = open(data_path, O_RDONLY, 664);
	if (fd == -1)
	{
		perror("open");
		exit(-1);
	}
	char buf[1024] = {};
	ssize_t size = read(fd, buf, sizeof(buf) - sizeof(buf[0]));
	if (size == -1)
	{
		perror("read");
		exit(-1);
	}
	printf("%s\n", buf);
	close(fd);

	//数据的处理
	data_t data;
	data.classes = atoi(strtok(buf, " "));
	data.x_point = atof(strtok(NULL, " "));
	data.y_point = atof(strtok(NULL, " "));
	data.x_len = atof(strtok(NULL, " "));
	data.y_len = atof(strtok(NULL, " "));
	printf("%d \nx_point = %lf\ny_point = %lf\nx_len   = %lf\ny_len   = %lf\n", data.classes, data.x_point, data.y_point, data.x_len, data.y_len);
	return data;
}
move_t disposal_data(data_t const data)
{
	//控制 x的速度和转向貌似就够
	double camera_img_size = 4.0 / 3.0;								   //图像尺寸比
	double camera_width_angle = 90.0;								   //相机宽度视野角
	double camera_height_angle = camera_width_angle * camera_img_size; //相机高度视野角
	double camera_floor_level_angle = 30.0;							   //相机与地面水平角
	double uva_takoff_h = 3.0;										   //无人机起飞高度，任务高度
	// double img_height = 3/tan((90-camera_floor_level_angle+camera_height_angle/2)*DEC) -
	// 					3/tan((90-camera_floor_level_angle-camera_height_angle/2)*DEC);
	// double img_width = 4/3*img_height;
	/*
	图像尺寸，img_w = 303; img_h = 204;
	相机高度 h = 30;
	w_angle = atrtan(img_w/2/30); //80
	h_angle = atrtan(imh_h/2/30); //76
	*/
	double k = 0.5;
	move_t move;
	if (data.y_point - k > -Y_OVERLOOK_K && data.y_point - k < Y_OVERLOOK_K)
	{ //目标在图像中心一定范围内，无动作
		move.x_vel = 0;
	}
	else if (data.y_point - k > Y_OVERLOOK_K)
	{ //目标在图像中心的下方，后退
		move.x_vel = -0.5;
	}
	else if (data.y_point - k < -Y_OVERLOOK_K)
	{ //目标在图像中心的上方，前进
		move.x_vel = 0.5;
	}
	if (data.x_point - k > -X_OVERLOOK_K && data.x_point - k < X_OVERLOOK_K)
	{ //目标在图像中心的一定范围内，无动作
		move.yaw = 0.0;
	}
	else if (data.x_point - k > X_OVERLOOK_K)
	{ //目标在图像中心的右边，顺序针偏航
		move.yaw = 1.0;
	}
	else if (data.x_point - k < -X_OVERLOOK_K)
	{ //目标在图像中心的左边，逆时针偏航
		move.yaw = -1.0;
	}
	printf("xv: %lf\tyaw: %lf\n", move.x_vel, move.yaw);
	return move;
}
int main(int argc, char *argv)
{
	//组织数据文件的路径
	char path[128] = "run_detect/labels/";
	int file_num = 458;
	int camere_num = 6;
	data_t data = getdata(path, file_num, camere_num, NULL);
	//数据换算
	move_t move = disposal_data(data);
	return 0;
}
