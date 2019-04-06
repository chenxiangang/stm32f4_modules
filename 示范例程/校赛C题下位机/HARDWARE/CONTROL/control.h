#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"


extern int t_mode ; //通信方式 PWM 0  还是 CAN 1 初始CAN
extern u8 out_mode;    //控制转速还是方向
extern u16 out_msg;    //输出信息

void SetAngle(double Setangle);
void motorback(void);
void TIM3_Int_Init(u16 arr, u16 psc);
void ClearAll(void);

#endif


