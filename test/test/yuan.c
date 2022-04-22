//文件的元数据
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
//转换类型和权限 -rw-rw-r--
char* mtos(mode_t m){
    static char s[11];
    if(S_ISDIR(m)){
        strcpy(s,"d");
    }else if(S_ISSOCK(m)){
        strcpy(s,"s");
    }else if(S_ISCHR(m)){
        strcpy(s,"c");
    }else if(S_ISBLK(m)){
        strcpy(s,"b");
    }else if(S_ISLNK(m)){
        strcpy(s,"l");
    }else if(S_ISSOCK(m)){
        strcpy(s,"s");
    }else{
        strcpy(s,"-");
    }

    strcat(s,m & S_IRUSR ? "r" : "-");
    strcat(s,m & S_IWUSR ? "w" : "-");
    strcat(s,m & S_IXUSR ? "x" : "-");
    strcat(s,m & S_IRGRP ? "r" : "-");
    strcat(s,m & S_IWGRP ? "w" : "-");
    strcat(s,m & S_IXGRP ? "x" : "-");
    strcat(s,m & S_IROTH ? "r" : "-");
    strcat(s,m & S_IWOTH ? "w" : "-");
    strcat(s,m & S_IXOTH ? "x" : "-");

    if(m & S_ISUID){
        s[3] = (s[3] == 'x') ? 's' : 'S';
    }
    if(m & S_ISGID){
        s[6] = (s[6] == 'x') ? 's' : 'S';
    }
    if(m & S_ISVTX){
        s[9] = (s[9] == 'x') ? 't' : 'T';
    }
    return s;
}

//转换时间字符串
char* ttos(time_t time){
    static char t[20];
    struct tm* l = localtime(&time); //
    sprintf(t,"%04d-%02d-%02d %02d:%02d:%02d",l->tm_year + 1900,l->tm_mon + 1,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec); //输出到存储区，（类似字符串拼接）
    return t;
}

int main(int argc,char* argv[]){
    // ./a.out  hello.c
    if(argc < 2){
        fprintf(stderr,"用法：./a.out <文件名>\n");
        return -1;
    }
    
    struct stat buf;//用来输出文件的元数据信息
    // if(stat(argv[1],&buf) == -1){
    if(stat("/home/xyd/workspace/my_code/code/run_detect/labels/6_123.txt",&buf) == -1){
        perror("stat");
        return -1;
    }
    
    printf("        设备ID：%lu\n",buf.st_dev);
    printf("       i节点号：%ld\n",buf.st_ino);
    printf("    类型和权限：%s\n",mtos(buf.st_mode));
    printf("      硬连接数：%lu\n",buf.st_nlink);
    printf("        用户ID：%u\n",buf.st_uid);
    printf("          组ID：%u\n",buf.st_gid);
    printf("    特殊设备ID：%lu\n",buf.st_rdev);
    printf("      总字节数：%ld\n",buf.st_size);
    printf("   I/O块字节数：%ld\n",buf.st_blksize);
    printf("      存储块数：%ld\n",buf.st_blocks);
    printf("  最后访问时间：%s\n",ttos(buf.st_atime));
    printf("  最后修改时间：%s\n",ttos(buf.st_mtime));
    printf("  最后改变时间：%s\n",ttos(buf.st_ctime));
    return 0;
}