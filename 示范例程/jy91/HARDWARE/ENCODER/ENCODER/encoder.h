#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (4150*4)  
//Ò»È¦ÐèÒª

void Encoder_TIM4_Init(void);
double Read_Encoder(void);
#endif
