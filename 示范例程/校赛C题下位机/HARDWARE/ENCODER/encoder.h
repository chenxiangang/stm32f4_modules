#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (65*4-1)  //���ݵ���������ģ�һȦ4345������ֵ��T12ģʽ���������Գ�4
//һȦ��Ҫ
extern double angle;
void Encoder_TIM4_Init(void);
double Read_Encoder(void);
#endif
