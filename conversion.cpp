#include <math.h>

#define PI 3.1415926
#define ANGTORAD_COE (PI/180)
#define RADTOANG_COE (180/PI)
#define IMG_CENTRAL_X 0.5
#define IMG_CENTRAL_Y 0.5
#define BASICS_VEL 0.5

typedef struct data{
    int cla;
    double x_point;
    double y_point;
    double x_len;
    double y_len;
}data_t;
typedef struct move{
    double x_v;
    double y_v;
    double z_v;
    double raw;
}move_t;

data_t data_ok;
move_t move_ok;

int main(void){
    double img_tall = 480; //ͼ���
    double img_wide = 640; //ͼ���
    double vide_angle = 120 * ANGTORAD_COE; //�����Ұ�ǣ�����
    double camera_ground_level_angle = 30 * ANGTORAD_COE; //���������ˮƽ�ǣ�����
    double nva_ground_tall = 0.0; //���˻������ĸ߶�

    double img_tall_ang = img_tall * vide_angle / sqrt(pow(img_tall,2) + pow(img_wide,2)); //ͼ��߶ȵ���Ұ��
    double img_wide_ang = img_wide * vide_angle / sqrt(pow(img_tall,2) + pow(img_wide,2)); //ͼ���ȵ���Ұ��

    move_ok.raw = IMG_CENTRAL_X - data_ok.x_point * img_wide_ang / img_wide;
    move_ok.x_v = (IMG_CENTRAL_Y - data_ok.y_point > 0 ? IMG_CENTRAL_Y - data_ok.y_poin : -(IMG_CENTRAL_Y - data_ok.y_poin)) + BASICS_VEL;
    return 0;
}


