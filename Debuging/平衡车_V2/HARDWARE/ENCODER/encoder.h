#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (802) //���ݵ���������ģ�һȦ4345������ֵ��T12ģʽ���������Գ�4
//һȦ��Ҫ

void Encoder_TIM4_Init(void);
void Encoder_TIM2_Init(void);
double Read_Encoder(u8 wheel);
#endif
