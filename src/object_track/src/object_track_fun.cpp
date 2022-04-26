#ifndef _OBJECT_TRACK_FUN_
#define _OBJECT_TRACK_FUN_
#include "object_track/object_track.hpp"
int struct_init(data_t* new_data, data_t* old_data, move_t* move){
    // new_data的初始化，假设刚开始的目标点在图像中心
	new_data->classes = 0;
	new_data->x_point = 0.5;
	new_data->y_point = 0.5;
	new_data->x_len = 0.1;
	new_data->y_len = 0.2;
	*old_data = *new_data;
    //初始化move
    move->x_vel = 0;
    move->y_vel = 0;
    move->z_vel = 0;
    move->yawr = 0;
    return 0;
}
int getnewfile(char *const path, char *newfile_path)
{
    DIR *dir = opendir(path);
    struct dirent *dirp = (struct dirent *)(malloc(sizeof(struct dirent)));
    struct stat *statp = (struct stat *)(malloc(sizeof(struct stat)));
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }
    time_t newfilemtime = 0; //最后修改的文件时间
    while ((dirp = readdir(dir)) != NULL)
    {
        // printf("%s\t", dirp->d_name); //输出目录中所有的文件名
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        { //跳过 . 和 .. 目录
            continue;
        }
        char file_path[512] = {};
        sprintf(file_path, "%s%s", path, dirp->d_name); //拼接完整的路径
        // printf("%s\n", file_path); //输出所有文件的相对路径

        //获取信息
        if (stat(file_path, statp) == -1)
        {
            perror("stat");
            return -1;
        }
        time_t file_mtime = statp->st_mtime; //当前stat获取文件的最后修改时间
        if (newfile_path[0] == '\0' || file_mtime > newfilemtime)
        {
            strcpy(newfile_path, file_path);
            newfilemtime = file_mtime;
        }
    }
#ifdef PRINTF_SWITCH
    printf(">>> file path:\n%s\n", newfile_path);
#endif
    //关闭目录，释放内存
    if (closedir(dir) == -1)
    {
        perror("closedir");
        return -1;
    }
    free(dirp);
    free(statp);
    return 0;
}

int readfiledata(char *const path, char *read_buf, int read_len)
{
    int fd = open(path, O_RDONLY, 664);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    ssize_t size = read(fd, read_buf, read_len);
    if (size == -1)
    {
        perror("read");
        return -1;
    }
    close(fd);
#ifdef PRINTF_SWITCH
    printf(">>> read data:\n%s", read_buf);
#endif
    return 0;
}

int tokdata(char *read_buf, data_t *data_buf)
{
    //对数据进行拆分，以'\n'分割，存入buf_data_link字符串数组中
    char buf_data_link[64][64] = {};
    char *str = strtok(read_buf, "\n"); //临时存储分割后的字符串
    if (str == NULL)
    {
        perror("strtok");
        return -1;
    }
    int linknum = 0; //记录检测到数据的个数
    while (str != NULL)
    {
        // printf(">> %s\n", str);
        // printf("buf_data_link+i->%p\nstr->%p\n", &buf_data_link[i], &str);
        strcpy(buf_data_link[linknum], str); //将分割后的字符从写入buf_data_link的一行中
        linknum++;
        str = strtok(NULL, "\n");
    }
    #ifdef PRINTF_SWITCH
    printf("n _ok!\n");
    #endif

    // printf("\n");
    //遍历输出buf_data_link
    // for(int i = 0; i < sizeof(buf_data_link)/sizeof(buf_data_link[0]) && buf_data_link[i][0] != '\0'; i++){
    // 	printf("<<  %p->%s\n", &buf_data_link[i], buf_data_link[i]);
    // }

    //将每个数据写入分别的结构体中
    // data_t buf_data[32]; //目标物体在图像中的数据信息，最大32个目标信息
    for (int i = 0; i < linknum; i++)
    {
        (data_buf + i)->classes = atoi(strtok(buf_data_link[i], " "));
        (data_buf + i)->x_point = atof(strtok(NULL, " "));
        (data_buf + i)->y_point = atof(strtok(NULL, " "));
        (data_buf + i)->x_len = atof(strtok(NULL, " "));
        (data_buf + i)->y_len = atof(strtok(NULL, " "));
        if (buf_data_link[i + 1][0] == '\0')
        {
            data_buf[i + 1].classes = -1;
            break;
        }
    }

    #ifdef PRINTF_SWITCH
    printf("ko_ok!\n");
    #endif
    //将检测到的目标遍历输出每个数据的信息，通过buf_data

#ifdef PRINTF_SWITCH
    // 遍历输出所读取到的数据
    printf(">>> data_tok:\n");
    for (int i = 0; data_buf[i].classes == 0; i++)
    {
        // for(int i = 0; i < data_buf_len && data_buf[i].classes == 0; i++){
        printf("%d\t%lf\t%lf\t%lf\t%lf\t\n", (data_buf + i)->classes, (data_buf + i)->x_point, (data_buf + i)->y_point, (data_buf + i)->x_len, (data_buf + i)->y_len);
    }
#endif
    return 0;
}

int get_torget_data(data_t *new_data, data_t *data_buf, data_t *old_data, int const data_buf_len)
{
    //数据判断，开始进行锁定目标
    int num_i = -1;
    double new_min_dist;
    double old_min_dist = 999;
    for (int i = 0; i < data_buf_len - 1 && data_buf[i].classes == 0; i++)
    {
        new_min_dist = sqrt(pow(fabs(old_data->x_point - data_buf[i].x_point), 2) + pow(fabs(old_data->y_point - data_buf[i].y_point), 2));
        // printf("new_min_dist = %lf\n", new_min_dist);
        if (new_min_dist < old_min_dist && new_min_dist <= TIME_DIST)
        {
            num_i = i;
            old_min_dist = new_min_dist;
        }
    }
    if (num_i == -1)
    {
        // #ifdef EN
        // printf(">>> No target was detected\n");
        // #else
        // printf(">>> 没有检测到目标\n");
        // #endif
        return -1;
    }
    //数据的写入
    new_data->classes = data_buf[num_i].classes;
    new_data->x_point = data_buf[num_i].x_point;
    new_data->y_point = data_buf[num_i].y_point;
    new_data->x_len = data_buf[num_i].x_len;
    new_data->y_len = data_buf[num_i].y_len;
#ifdef PRINTF_SWITCH
    printf("nwe and old:\n");
    printf("time\tcla\tx_point\t\ty_point\t\tx_len\t\ty_len\n");
    printf("new\t%d\t%lf\t%lf\t%lf\t%lf\n", new_data->classes, new_data->x_point, new_data->y_point, new_data->x_len, new_data->y_len);
    printf("old\t%d\t%lf\t%lf\t%lf\t%lf\n", old_data->classes, old_data->x_point, old_data->y_point, old_data->x_len, old_data->y_len);
#endif
    return 0;
}

int data_disposal(move_t *move, data_t *const data)
{
    double img_tall = 480;                                                                   //图像高
    double img_wide = 640;                                                                   //图像宽
    double vide_angle = 120 * ANGTORAD_COE;                                                  //相机视野角，弧度
    double camera_ground_level_angle = 30 * ANGTORAD_COE;                                    //相机与地面的水平角，弧度
    double nva_ground_tall = 0.0;                                                            //无人机离地面的高度
    double img_tall_ang = img_tall * vide_angle / sqrt(pow(img_tall, 2) + pow(img_wide, 2)); //图像高度的视野角
    double img_wide_ang = img_wide * vide_angle / sqrt(pow(img_tall, 2) + pow(img_wide, 2)); //图像宽度的视野角

    //初始化move
    move->x_vel = 0;
    move->y_vel = 0;
    move->z_vel = 0;
    move->yawr = 0;

    double k = 0.5;
    if (data->y_point - k > -Y_OVERLOOK_K && data->y_point - k < Y_OVERLOOK_K)
    { //目标在图像中心一定范围内，无动作
        move->x_vel = 0;
    }
    else if (data->y_point - k > Y_OVERLOOK_K)
    { //目标在图像中心的下方，后退
        move->x_vel = -(fabs(data->y_point - k)*5+1);
        // move->x_vel = -1;
    }
    else if (data->y_point - k < -Y_OVERLOOK_K)
    { //目标在图像中心的上方，前进
        move->x_vel = fabs(data->y_point - k)*5+1;
        // move->x_vel = 1;
    }
    if (data->x_point - k > -X_OVERLOOK_K && data->x_point - k < X_OVERLOOK_K)
    { //目标在图像中心的一定范围内，无动作
        move->yawr = 0.0;
    }
    else if (data->x_point - k > X_OVERLOOK_K)
    { //目标在图像中心的右边，顺序针偏航
        move->yawr = -0.25;
        // move->yawr = -((data->x_point - k)*5+1);
    }
    else if (data->x_point - k < -X_OVERLOOK_K)
    { //目标在图像中心的左边，逆时针偏航
        move->yawr = 0.25;
        // move->yawr = (data->x_point - k)*5+1;
    }
    // printf("xv: %lf\tyaw: %lf\n", move->x_vel, move->yaw);
    return 0;
}
int data_readtodisp(data_t *new_data, data_t *old_data, char *const exp_file_path, move_t *move)
{
    if (new_data->classes == 0)
    {
        *old_data = *new_data;
    }
    //获取文件
    char data_path[128] = {};
    if (getnewfile(exp_file_path, data_path) == -1)
    {
        new_data->classes = -1;
        return -1;
    }
    #ifdef PRINTF_SWITCH
    printf("getfile_ok!\n");
    #endif
    //读取文件
    char data_buf[128] = {};
    if (readfiledata(data_path, data_buf, sizeof(data_buf)) == -1)
    {
        new_data->classes = -1;
        return -1;
    }
    #ifdef PRINTF_SWITCH
    // printf("readfile_ok!\n");
    #endif
    //分割数据
    data_t data_tok[32];
    if (tokdata(data_buf, data_tok) == -1)
    {
        new_data->classes = -1;
        return -1;
    }
    #ifdef PRINTF_SWITCH
    // printf("tok_ok!\n");
    #endif
    //数据锁定
    if (get_torget_data(new_data, data_tok, old_data, sizeof(data_tok) / sizeof(data_t)) == -1)
    {
        new_data->classes = -1;
        return -1;
    }
    #ifdef PRINTF_SWITCH
    // printf("torget_ok!\n");
    #endif
    //数据处理
    if (data_disposal(move, new_data) == -1)
    {
        return -1;
    }
    #ifdef PRINTF_SWITCH
    // printf("disp_ok!\n");
    #endif
    return 0;
}

#endif