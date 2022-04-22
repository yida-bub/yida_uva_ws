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
	//拼接数据文件的路径到 data_path中
	sprintf(data_path, "%s%d_%d.txt", path, camere_num, file_num);
	//数据文件的读取，将读取到的数据暂存入buf中
	char buf[1024] = {};
	int fd = open(data_path, O_RDONLY, 664);
	if (fd == -1)
	{
		// perror("open");
		new_data->classes = -1;
		return -1;
	}
	printf("open data path: %s\n", data_path); //输出当前数据文件的路径
	ssize_t size = read(fd, buf, sizeof(buf) - sizeof(buf[0]));
	if (size == -1)
	{
		perror("read");
		new_data->classes = -1;
		return -1;
	}
	printf("%s\n", buf); //输出读取到的数据
	close(fd);
	//对数据进行拆分，以'\n'分割，存入buf_data_link字符串数组中
	char buf_data_link[32][64] = {};
	char *str = strtok(buf, "\n"); //临时存储分割后的字符串
	int linknum = 0; //记录检测到数据的个数
	while(str != NULL){
		// printf(">> %s\n", str);
		// printf("buf_data_link+i->%p\nstr->%p\n", &buf_data_link[i], &str);
		strcpy(buf_data_link[linknum], str); //将分割后的字符从写入buf_data_link的一行中
		linknum++; 
		str = strtok(NULL, "\n");
	}
	printf("\n");
	// //遍历输出buf_data_link
	// for(int i = 0; i < sizeof(buf_data_link)/sizeof(buf_data_link[0]) && buf_data_link[i][0] != '\0'; i++){
	// 	printf("<<  %p->%s\n", &buf_data_link[i], buf_data_link[i]);
	// }

	//将每个数据写入分别的结构体中
	data_t buf_data[32]; //目标物体在图像中的数据信息，最大32个目标信息
	for(int i = 0; i < linknum; i++){
		(buf_data+i)->classes = atoi(strtok(buf_data_link[i], " "));
		(buf_data+i)->x_point = atof(strtok(NULL, " "));
		(buf_data+i)->y_point = atof(strtok(NULL, " "));
		(buf_data+i)->x_len = atof(strtok(NULL, " "));
		(buf_data+i)->y_len = atof(strtok(NULL, " "));
		if(buf_data_link[i+1][0] == '\0'){
			buf_data[i+1].classes = -1;
			break;
		}
	}
	//遍历输出每个数据结构
	printf("read data:\n");
	for(int i = 0; i < sizeof(buf_data)/sizeof(buf_data[0]) && buf_data[i].classes == 0; i++){
		printf("%d\t%lf\t%lf\t%lf\t%lf\t\n", (buf_data+i)->classes, (buf_data+i)->x_point, (buf_data+i)->y_point, (buf_data+i)->x_len, (buf_data+i)->y_len);
	}

	//数据判断，开始进行锁定目标
	int num_i = -1;
	// int num_i = 0;
	for(int i = 0; i < sizeof(buf_data)/sizeof(buf_data[0]) && buf_data[i].classes == 0; i++){
		if(fabs(new_data->x_point-buf_data[i].x_point) < 0.1 || fabs(new_data->y_point-buf_data[i].y_point) < 0.2){
			//判断需完善
			num_i = i;
		}
	}
	if(num_i == -1){
		new_data->classes = -1;
		return 0;
	}
	new_data->classes = buf_data[num_i].classes;
	new_data->x_point = buf_data[num_i].x_point;
	new_data->y_point = buf_data[num_i].y_point;
	new_data->x_len = buf_data[num_i].x_len;
	new_data->y_len = buf_data[num_i].y_len;

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

	//new_data的初始化，假设刚开始的目标点在图像中心
	new_data->classes = 0;
	new_data->x_point = 0.5;
	new_data->y_point = 0.5;
	new_data->x_len = 0.1;
	new_data->y_len = 0.2;

	for (int i = 0;; i++)
	{
		if(i != 0 && new_data->classes == 0){
			*old_data = *new_data;
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
