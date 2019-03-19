#ifndef __TB6612_H
#define __TB6612_H
#include "sys.h"



//LED端口定义
#define AIN1 PAout(0)	// AIN1
#define AIN2 PAout(1)	// AIN2	 
#define BIN1 PAout(2)	// BIN1	 
#define BIN2 PAout(3)	// BIN2	 


void TB6612_Init(void);//初始化		 
void TIM3_PWM_Init(void);
void TB6612_Init(void);
void speedcontrl(int speed,int ch);

#endif

