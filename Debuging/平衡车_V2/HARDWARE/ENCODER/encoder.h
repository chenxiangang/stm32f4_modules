#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (802) //根据电机本身来改，一圈4345个编码值，T12模式计数，所以乘4
//一圈需要

void Encoder_TIM4_Init(void);
void Encoder_TIM2_Init(void);
double Read_Encoder_L(void);
double Read_Encoder_R(void);
#endif
