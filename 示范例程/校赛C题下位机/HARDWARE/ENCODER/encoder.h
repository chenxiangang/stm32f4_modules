#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (65*4-1)  //根据电机本身来改，一圈4345个编码值，T12模式计数，所以乘4
//一圈需要
extern double angle;
void Encoder_TIM4_Init(void);
double Read_Encoder(void);
#endif
