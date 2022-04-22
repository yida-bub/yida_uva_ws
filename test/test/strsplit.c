#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
int strsplit(char* const instr, char c, char *outstr){
	//231231 123123 123123 123
	//c = " "
	int count = 0;
	char buf[128] = {};
	int buf_i = 0;
	for(int i = 0; ;i++){
		if(instr[i] == c || instr[i] == '\0'){
            buf[buf_i] = '\0';
            printf("buf = %s\n", buf);
            strcpy(outstr+(count*20), buf);
			count++;
			buf_i = 0;
            if(instr[i] == '\0'){
                break;
            }
            continue;
		}
		buf[buf_i] = instr[i];
		buf_i++;
	}
    // for(char *p = outstr; *p; p = p + size){
    //     printf("%s     ", p);
    // }
    printf("\n");
    // return len;
}
int main(void){
    char* instr = "12345 \n67890 yoiue \ndfasdks sda mncx";
    // char outstr[16][16] = {};
    // printf("1");
    // char *outstr = malloc(0);
    char outstr[20][10] = {};
    strsplit(instr, '\n', &outstr);
    printf("%s\n", outstr[0]);
    printf("%s\n", outstr[1]);
    printf("%s\n", outstr[2]);
    printf("%s\n", outstr[3]);

    // for(char *p = outstr; *p; p = p + sizeof(outstr)*si){
    //     printf("111");
    //     printf("%s ", p);
    // }
    // printf("%lu %lu\n", sizeof(char*), sizeof(void**));
    // strcpy(outstr, "123456789999");
    // strcpy(outstr+8, "098");
    // strcpy(outstr+16, "231");
    // outstr = realloc(outstr, 32);
    // printf("%p  %s\n", outstr, outstr);
    // printf("%p  %s\n", outstr+8, outstr+8);
    // printf("%p  %s\n", outstr+16, outstr+16);
    // printf("%p  %s\n", outstr+24, outstr+24);
    // free(outstr);
    // strsplit(instr, ' ', outstr);

    // char buf[64] = "123 \n12312\n231";
    // printf("%s\n", buf);

    // char testbuf[][16] = {"1231", "2345", "3456", "45657"};
    // printf("%p\n", testbuf);
    // printf("%p\n", *testbuf);
    // printf("%p\n", *testbuf+1);
    // printf("%p\t%s\n", testbuf+1, *(testbuf+1));//2345
    return 0;
}