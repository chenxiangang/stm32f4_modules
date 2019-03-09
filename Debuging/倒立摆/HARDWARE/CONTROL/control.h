#ifndef __CONTROL_H
#define __CONTROL_H	
#include "sys.h"
#include "pid.h"
 
#define IN3 PAout(11)    //IN3 = 0,IN4 = 1为逆时针转动
#define IN4 PAout(12)
void Start_Swing(void);
u16 get_cal_sym(void);
void motor_action(int speed);
void Revert(void);

#endif 
