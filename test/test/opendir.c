#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
void uptodatefile(char* const path, char* newfile_path){
    DIR* dir = opendir(path);
    struct dirent *dirp = malloc(sizeof(struct dirent));
    struct stat *statp = malloc(sizeof(struct stat));
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
        printf("%s\n", file_path); //输出所有文件的相对路径
       
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
    // printf("nwe file:%s\n", newfilename);
    //关闭目录，释放内存
    if(closedir(dir) == -1){
        perror("closedir");
        exit(-1);
    }
    free(dirp);
    free(statp);
}
int main(void){
    char path[64] = "../run_detect/labels/";
    char newfile_path[64] = {};
    uptodatefile(path, newfile_path);
    printf("nwe file:%s\n", newfile_path);
    return 0;
}