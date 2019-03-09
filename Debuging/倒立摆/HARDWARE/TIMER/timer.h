#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "pid.h"


extern int speed;
extern int last_speed;
extern u16 pwmduty;
extern u16 realpwmduty;
extern double dutychange2;
extern double    rout;
extern u16 uccr1;
extern u16 nccr1;
extern PID sPID;
extern double position_cal_duty;


void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc); 
#endif
