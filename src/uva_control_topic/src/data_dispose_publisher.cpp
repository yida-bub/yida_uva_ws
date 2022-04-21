/*
功能：对yolo检测出的数据进行处理，再转换成无人机运动数据
说明：加入最新文件的判断
	 请使用c++编译程序，如：g++ read1.0.c -o read1.0
*/
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>	//文件读写
#include <dirent.h>
#include <string.h> //字符串操作
#include <sys/stat.h>
#include <sys/types.h>
#include <ros/ros.h>
#include "uva_control_topic/msg_data_struct.h"

//宏定义
#define PI 3.1415926535898
#define DEC (PI / 180)
#define Y_OVERLOOK_K 0.20 //图像中心点y坐标无动作范围	
#define X_OVERLOOK_K 0.12 //图像中心点x坐标无动作范围
#define LANGUAGE ZH //设置信息输出的语言：EH/ZH
#define TIME_DIST 0.8 //目标在图像内可允许移动的最大距离
#define YOLO_RUNS_DETECT_PATH "/home/xyd/workspace/yolov5/runs/detect/"

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

//获取目录中最后修改时间的文件路径
extern void uptodatefile(char* const path, char* newfile_path);
//读取文件的数据
extern int readfiledata(data_t* new_data, char* const path, char *read_buf, int read_buf_len);
//对数据进行拆分处理，输出到 buf_data 上
extern int data_split_clearup(char *read_buf, data_t *data_buf, int const data_buf_len);
//再次进行数据处理，获取目标的位置信息，输出到 new_data 上
extern int confirm_torget_data(data_t* new_data, data_t* buf_data, data_t* olddata, int const buf_data_len);
//数据总处理fun
extern int getdata(data_t* new_data, char *const path, data_t* const olddata);
extern int disposal_data(move_t* move, data_t* const data);
//获取yolo runs 运行的最新exp文件路径
int new_runs_catalogue_path(char* const path, char* newfile_path);
//自己的sleep休眠阻塞函数，单位ms
extern int sleep_ms(int time);

int sleep_ms(int time)
{
	for(int i = 0; i < time; i++){
		usleep(1000);
	}
	return 0;
}
void uptodatefile(char* const path, char* newfile_path){
    DIR* dir = opendir(path);
    struct dirent *dirp = (struct dirent*)(malloc(sizeof(struct dirent)));
    struct stat *statp = (struct stat*)(malloc(sizeof(struct stat)));
    if(dir == NULL){
        perror("opendir");
        exit(-1);
    }
    time_t newfilemtime = 0; //最后修改的文件时间
    while((dirp = readdir(dir)) != NULL){
        // printf("%s\t", dirp->d_name); //输出目录中所有的文件名
        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0 ){ //跳过 . 和 .. 目录
            continue;
        }
        char file_path[512] = {};
	    sprintf(file_path, "%s%s", path, dirp->d_name);//拼接完整的路径
        // printf("%s\n", file_path); //输出所有文件的相对路径
       
        //获取信息
        if(stat(file_path, statp) == -1){
            perror("stat");
            exit(-1);
        }
        time_t file_mtime = statp->st_mtime; //当前stat获取文件的最后修改时间
        if(newfile_path[0] == '\0' || file_mtime > newfilemtime){
            strcpy(newfile_path, file_path);
            newfilemtime = file_mtime;
        }
    }
    // printf("nwe file path:%s\n", newfile_path);
    //关闭目录，释放内存
    if(closedir(dir) == -1){
        perror("closedir");
        exit(-1);
    }
    free(dirp);
    free(statp);
}
int readfiledata(data_t* new_data, char* const path, char *read_buf, int read_buf_len){
	int fd = open(path, O_RDONLY, 664);
	if (fd == -1)
	{
		// perror("open");
		new_data->classes = -1;
		return -1;
	}

	// printf("*************************\n");
	// #ifdef EN
	// printf("[INFO] Path to the open data file: %s\n", data_path);
	// #else 
	// printf("[INFO] 打开的数据文件路径: %s\n", path);//输出当前数据文件的路径
	// #endif

	ssize_t size = read(fd, read_buf, read_buf_len);
	if (size == -1)
	{
		perror("read");
		new_data->classes = -1;
		return -1;
	}

	// #ifdef EN
	// printf("[INFO] Data read successfully\n", data_path);
	// #else 
	// printf("[INFO] 数据读取成功\n");//输出当前数据文件的路径
	// #endif

	// printf("%s\n", read_buf); //输出读取到的数据
	close(fd);
	return 0;
}
int data_split_clearup(char *read_buf, data_t *data_buf, int const data_buf_len){
	//对数据进行拆分，以'\n'分割，存入buf_data_link字符串数组中
	char buf_data_link[32][64] = {};
	char *str = strtok(read_buf, "\n"); //临时存储分割后的字符串
	if(str == NULL){
		perror("strtok");
		return -1;
	}
	int linknum = 0; //记录检测到数据的个数
	while(str != NULL){
		// printf(">> %s\n", str);
		// printf("buf_data_link+i->%p\nstr->%p\n", &buf_data_link[i], &str);
		strcpy(buf_data_link[linknum], str); //将分割后的字符从写入buf_data_link的一行中
		linknum++; 
		str = strtok(NULL, "\n");
	}

	// printf("\n");
	//遍历输出buf_data_link
	// for(int i = 0; i < sizeof(buf_data_link)/sizeof(buf_data_link[0]) && buf_data_link[i][0] != '\0'; i++){
	// 	printf("<<  %p->%s\n", &buf_data_link[i], buf_data_link[i]);
	// }

	//将每个数据写入分别的结构体中
	// data_t buf_data[32]; //目标物体在图像中的数据信息，最大32个目标信息
	for(int i = 0; i < linknum; i++){
		(data_buf+i)->classes = atoi(strtok(buf_data_link[i], " "));
		(data_buf+i)->x_point = atof(strtok(NULL, " "));
		(data_buf+i)->y_point = atof(strtok(NULL, " "));
		(data_buf+i)->x_len = atof(strtok(NULL, " "));
		(data_buf+i)->y_len = atof(strtok(NULL, " "));
		if(buf_data_link[i+1][0] == '\0'){
			data_buf[i+1].classes = -1;
			break;
		}
	}
	//将检测到的目标遍历输出每个数据的信息，通过buf_data

	// #ifdef EN
	// printf("[INFO] Data read:\n");
	// #else 
	// printf("[INFO] 读取到的数据\n");
	// #endif

	// // 遍历输出所读取到的数据
	// for(int i = 0; i < data_buf_len && data_buf[i].classes == 0; i++){
	// 	printf("%d\t%lf\t%lf\t%lf\t%lf\t\n", (data_buf+i)->classes, (data_buf+i)->x_point, (data_buf+i)->y_point, (data_buf+i)->x_len, (data_buf+i)->y_len);
	// }
	return 0;
}
int confirm_torget_data(data_t* new_data, data_t* buf_data, data_t* olddata, int const buf_data_len){
	//数据判断，开始进行锁定目标
	int num_i = -1;
	double new_min_dist;
	double old_min_dist = 999;
	for(int i = 0; i < buf_data_len-1 && buf_data[i].classes == 0; i++){
		new_min_dist = sqrt(pow(fabs(olddata->x_point - buf_data[i].x_point),2) + pow(fabs(olddata->y_point - buf_data[i].y_point),2));
		// printf("new_min_dist = %lf\n", new_min_dist);
		if(new_min_dist < old_min_dist && new_min_dist <= TIME_DIST){ 
			num_i = i;
			old_min_dist = new_min_dist;
		}
	}
	if(num_i == -1){
		new_data->classes = -1;

		// #ifdef EN
		// printf("[INFO] No target was detected\n");
		// #else 
		// printf("[INFO] 没有检测到目标\n");
		// #endif

		return 0;
	}
	//数据的写入
	new_data->classes = buf_data[num_i].classes;
	new_data->x_point = buf_data[num_i].x_point;
	new_data->y_point = buf_data[num_i].y_point;
	new_data->x_len = buf_data[num_i].x_len;
	new_data->y_len = buf_data[num_i].y_len;
	return 0;
}
int getdata(data_t* new_data, char *const path, data_t* olddata)
{	
	//获取文件中最后修改的文件路径，考虑是否要忽略目标没有检测到时，最新文件则取消匹配
	char newfile_path[128] = {};
	uptodatefile(path, newfile_path);
	printf("%s\n", newfile_path);

	//将从文件中读到的数据存入write_buf中
	char read_buf[1024] = {};
	if(readfiledata(new_data, newfile_path, read_buf, sizeof(read_buf)) != 0){
		return -1;
	}
	
	//对数据进行拆分处理，输出到 buf_data 上
	data_t buf_data[32]; //目标物体在图像中的数据信息，最大32个目标信息
	if(data_split_clearup(read_buf, buf_data, sizeof(buf_data)) != 0){
		return -1;
	}

	//再次进行数据处理，获取目标的位置信息，输出到 new_data 上
	confirm_torget_data(new_data, buf_data, olddata, sizeof(buf_data)/sizeof(data_t));

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
int new_runs_catalogue_path(char* const path, char* newfile_path){
	// char newfile_path[128] = {};
	uptodatefile(path, newfile_path);
	strcat(newfile_path, "/labels/");
	return 0;
}
int main(int argc, char **argv)
{
	
	//组织数据文件的路径
	char path[128] = YOLO_RUNS_DETECT_PATH;
	char newfile_path[128] = {};
	new_runs_catalogue_path(path, newfile_path);
	// printf("%s\n", newfile_path);

	int file_num = 0;
	int camere_num = 6;
	data_t* new_data = (data_t*)(malloc(sizeof(data_t)));
	data_t* old_data = (data_t*)(malloc(sizeof(data_t)));
	move_t* move = (move_t*)(malloc(sizeof(move_t)));

	//new_data的初始化，假设刚开始的目标点在图像中心
	new_data->classes = 0;
	new_data->x_point = 0.5;
	new_data->y_point = 0.5;
	new_data->x_len = 0.1;
	new_data->y_len = 0.2;
	*old_data = *new_data;

    // ROS节点初始化
    ros::init(argc, argv, "data_publisher");
    // 创建节点句柄
    ros::NodeHandle n;
    // 创建一个Publisher，发布名为/data_info的topic，消息类型为conrol_pkg::message_data，队列长度10
    ros::Publisher data_info_pub = n.advertise<uva_control_topic::msg_data_struct>("/data_info", 10);
    // 设置循环的频率
    ros::Rate loop_rate(1);

	while (ros::ok())
	{
		if(new_data->classes == 0){
			*old_data = *new_data;
		}
		//数据读取，及数据的处理，输出到data上
		getdata(new_data, newfile_path, old_data);
		if(new_data->classes != 0){
			//add............无速度
			continue;
		}
		//数据换算
		disposal_data(move, new_data);
		//输出信息
		// printf("time\tcla\tx_point\t\ty_point\t\tx_len\t\ty_len\n");
		// printf("new\t%d\t%lf\t%lf\t%lf\t%lf\n", new_data->classes, new_data->x_point, new_data->y_point, new_data->x_len, new_data->y_len);
		// printf("old\t%d\t%lf\t%lf\t%lf\t%lf\n", old_data->classes, old_data->x_point, old_data->y_point, old_data->x_len, old_data->y_len);
		// printf("xv: %lf\tyaw: %lf\n", move->x_vel, move->yaw);

		//初始化conrol_pkg::message_data类型的消息
		uva_control_topic::msg_data_struct data_msg;
		data_msg.x_vel = move->x_vel;
		data_msg.y_vel = 0;
		data_msg.z_vel = 0;
		data_msg.yaw = move->yaw;
		//发布消息
		data_info_pub.publish(data_msg);
		ROS_INFO("x_vel:%lf  y_vel:%lf  z_vel:%lf  yaw:%lf", data_msg.x_vel, data_msg.y_vel, data_msg.z_vel, data_msg.yaw);
		// 按照循环频率延时
		loop_rate.sleep();

		// sleep_ms(500);
		// sleep();
		// printf("*************************\n");
	}
	return 0;
}
