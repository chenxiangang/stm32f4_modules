#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern int Left_Encoder_Angle; //左轮编码器读到的角度
extern int Right_Encoder_Angle; //右轮编码器读到的角度
extern double last_Left_Encoder_Angle; //左轮编码器读到的角度
extern double last_Right_Encoder_Angle; //右轮编码器读到的角度
extern int taskMode; //根据题目选择不同的模式
extern double balance_point;

extern double pwmduty1;
extern double pwmduty2;
extern int turn_flag;
extern int turn_speed;
extern int forward_flag;
extern int forward_speed;


#define remote_control 1
#define keep_balance 2
#define tracking 3

#define LEFT 1
#define RIGHT 0


void TIM5_Init(u16 arr, u16 psc);
void changeTask(u8 taskid);
void TIM7_Init(void);
#endif
