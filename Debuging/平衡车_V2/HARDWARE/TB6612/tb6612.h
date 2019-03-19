#ifndef __TB6612_H
#define __TB6612_H
#include "sys.h"

#define LeftWheel 1
#define RightWheel 2

//LED端口定义
#define AIN1 PAout(4) // AIN1
#define AIN2 PAout(5) // AIN2
#define BIN1 PAout(2) // BIN1
#define BIN2 PAout(3) // BIN2

void TB6612_Init(void); //初始化
void TIM3_PWM_Init(void);
void TB6612_Init(void);
void speedcontrol(double speed, int ch, float basepwm);

#endif
