#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern int Left_Encoder_Angle; //左轮编码器读到的角度
extern int Right_Encoder_Angle; //右轮编码器读到的角度
extern double balance_point;

extern double pwmduty1;
extern double pwmduty2;
extern int turn_flag;
extern int turn_speed;
extern int forward_flag;
extern int forward_speed;
extern u8 track_flag;

extern int corner_num;
extern int cross_num;

extern void forward(u16 flag, u16 dist);
extern void turn(u16 flag, u16 dist);
extern void stop_ctl();

#define LEFT 1
#define RIGHT 0

void TIM5_Init(u16 arr, u16 psc);
void TIM7_Init(void);
#endif
