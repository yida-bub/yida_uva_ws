#ifndef _OBJECT_TRACK_
#define _OBJECT_TRACK_

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

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>  //发布的消息体对应的头文件，该消息体的类型为geometry_msgs：：PoseStamped
#include <mavros_msgs/CommandBool.h>  //CommandBool服务的头文件，该服务的类型为mavros_msgs：：CommandBool
#include <mavros_msgs/SetMode.h>     //SetMode服务的头文件，该服务的类型为mavros_msgs：：SetMode
#include <mavros_msgs/State.h>  //订阅的消息体的头文件，该消息体的类型为mavros_msgs：：State
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/PoseStamped.h>

#define PI 3.1415926535898
#define ANGTORAD_COE (PI/180)
#define RADTOANG_COE (180/PI)
#define Y_OVERLOOK_K 0.20 //图像中心点y坐标无动作范围	
#define X_OVERLOOK_K 0.12 //图像中心点x坐标无动作范围
#define LANGUAGE ZH //设置信息输出的语言：EH/ZH
#define TIME_DIST 0.5 //目标在图像内可允许移动的最大距离
#define YOLO_RUNS_DETECT_PATH "/home/xyd/workspace/yolov5/runs/detect/"
#define PRINTF_SWITCH

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
	double yawr;	  //对应 data 的 x_point, 无人机航向角，及偏航
} move_t;

//获取最新的文件或目录，输出参数 newfile_path
extern int getnewfile(char* const path, char* newfile_path);
//打开，读取数据，失败返回-1
extern int readfiledata(char* const path, char* read_buf, int const read_len);
//对读取到的数据进行分割，并存入data_t类型的结构中
extern int tokdata(char* read_buf, data_t* data_buf);
//数据进行处理，获取锁定的目标位置信息，输出到 nwe_data 上
extern int get_torget_data(data_t* new_data, data_t* data_buf, data_t* olddata, int const data_buf_len);
//数据的转换
extern int data_disposal(move_t* move, data_t* const data);
#endif