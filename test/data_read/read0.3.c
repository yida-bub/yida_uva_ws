/*
功能：对yolo检测出的数据进行处理，再转换成无人机运动数据
说明：单一的变化，支持但个目标
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>	//文件读写
#include <string.h> //字符串操作

#define PI 3.1415926535898
#define DEC (PI / 180)
#define Y_OVERLOOK_K 0.20 //图像中心点y坐标无动作范围
#define X_OVERLOOK_K 0.12 //图像中心点x坐标无动作范围

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
int sleep_ms(int time)
{
	for(int i = 0; i < time; i++){
		usleep(1000);
	}
	return 0;
}
int getdata(data_t* new_data, char *const path, int const file_num, int const camere_num, data_t* const olddata)
{
	//组织数据文件的路径
	char data_path[256] = {};
	// new_data = {};
	sprintf(data_path, "%s%d_%d.txt", path, camere_num, file_num);
	//数据的读取
	int fd = open(data_path, O_RDONLY, 664);
	if (fd == -1)
	{
		// perror("open");
		new_data->classes = -1;
		return -1;
	}
	printf("open data path: %s\n", data_path);
	char buf[1024] = {};
	ssize_t size = read(fd, buf, sizeof(buf) - sizeof(buf[0]));
	if (size == -1)
	{
		perror("read");
		new_data->classes = -1;
		return -1;
	}
	printf("%s\n", buf);
	close(fd);
	// remove(data_path); //删除刚读取的数据文件
	//获取目标数据个数
	char bufpp[20][64] = {};
	// int linknum = 0;
	// for(int i = 0; ;i++){
	// 	if(buf[i] == '\0'){
	// 		break;
	// 	}
	// 	if(buf[i] == '\n'){
	// 		linknum++;
	// 	}
	// 	//////////////////////////////
	// }
	// printf("linknum = %d", linknum);
	//数据的处理
	for(int i = 0; i < sizeof(bufpp)/sizeof(bufpp[0]); i++){
	// for(int i = 0; bufpp[i+2] != NULL; i++){
		if(i=0){
			strcpy(bufpp[0], strtok(buf, "\n"));
		}
		strcpy(bufpp[i+1], strtok(NULL, "\n"));
	}
	for(int i = 0; i < sizeof(bufpp)/sizeof(bufpp[0]); i++){
	// for(int i = 0; bufpp[i] != NULL; i++){
		printf("%s\n", bufpp[i]);
	}
	new_data->classes = atoi(strtok(bufpp[0], " "));
	new_data->x_point = atof(strtok(NULL, " "));
	new_data->y_point = atof(strtok(NULL, " "));
	new_data->x_len = atof(strtok(NULL, " "));
	new_data->y_len = atof(strtok(NULL, " "));


	// new_data->classes = atoi(strtok(buf, " "));
	// new_data->x_point = atof(strtok(NULL, " "));
	// new_data->y_point = atof(strtok(NULL, " "));
	// new_data->x_len = atof(strtok(NULL, " "));
	// new_data->y_len = atof(strtok(NULL, " "));
	// printf("%d \nx_point = %lf\ny_point = %lf\nx_len   = %lf\ny_len   = %lf\n", new_data->classes, new_data->x_point, new_data->y_point, new_data->x_len, new_data->y_len);
	return 0;
}
int disposal_data(move_t* move, data_t* const data)
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
	if (data->y_point - k > -Y_OVERLOOK_K && data->y_point - k < Y_OVERLOOK_K)
	{ //目标在图像中心一定范围内，无动作
		move->x_vel = 0;
	}
	else if (data->y_point - k > Y_OVERLOOK_K)
	{ //目标在图像中心的下方，后退
		move->x_vel = -0.5;
	}
	else if (data->y_point - k < -Y_OVERLOOK_K)
	{ //目标在图像中心的上方，前进
		move->x_vel = 0.5;
	}
	if (data->x_point - k > -X_OVERLOOK_K && data->x_point - k < X_OVERLOOK_K)
	{ //目标在图像中心的一定范围内，无动作
		move->yaw = 0.0;
	}
	else if (data->x_point - k > X_OVERLOOK_K)
	{ //目标在图像中心的右边，顺序针偏航
		move->yaw = 1.0;
	}
	else if (data->x_point - k < -X_OVERLOOK_K)
	{ //目标在图像中心的左边，逆时针偏航
		move->yaw = -1.0;
	}
	// printf("xv: %lf\tyaw: %lf\n", move->x_vel, move->yaw);
	return 0;
}
int main(int argc, char *argv)
{
	//组织数据文件的路径
	char path[128] = "../run_detect/labelscopy/";
	int file_num = 0;
	int camere_num = 6;
	data_t* new_data = malloc(sizeof(data_t));
	data_t* old_data = malloc(sizeof(data_t));
	move_t* move = malloc(sizeof(move_t));
	for (int i = 0;; i++)
	{
		if(i != 0 && new_data->classes == 0){
			*old_data = *new_data;
			// free(new_data);
			// new_data = malloc(sizeof(data_t));
		}
		//数据读取，及数据的处理，输出到data上
		getdata(new_data, path, i, camere_num, old_data);
		if(new_data->classes != 0){
			continue;
		}
		//数据换算
		disposal_data(move, new_data);
		//输出信息
		printf("*************************\n");
		printf("time\tcla\tx_point\t\ty_point\t\tx_len\t\ty_len\n");
		printf("new\t%d\t%lf\t%lf\t%lf\t%lf\n", new_data->classes, new_data->x_point, new_data->y_point, new_data->x_len, new_data->y_len);
		printf("old\t%d\t%lf\t%lf\t%lf\t%lf\n", old_data->classes, old_data->x_point, old_data->y_point, old_data->x_len, old_data->y_len);
		printf("xv: %lf\tyaw: %lf\n", move->x_vel, move->yaw);
		printf("*************************\n");
		sleep_ms(100);
	}
	return 0;
}
