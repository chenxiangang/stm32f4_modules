#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern double Left_Encoder_Angle; //左轮编码器读到的角度
extern double Right_Encoder_Angle; //右轮编码器读到的角度
extern double last_Left_Encoder_Angle; //左轮编码器读到的角度
extern double last_Right_Encoder_Angle; //右轮编码器读到的角度
extern int taskMode; //根据题目选择不同的模式

extern double pwmduty;

#define remote_control 1
#define keep_balance 2
#define tracking 3

void TIM5_Init(u16 arr, u16 psc);
void changeTask(u8 taskid);

#endif
