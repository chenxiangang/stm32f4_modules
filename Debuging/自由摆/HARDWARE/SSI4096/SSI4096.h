#ifndef _SSI4096_H_
#define _SSI4096_H_
#include "sys.h"
#include "stm32f4xx.h"
#define SSI_CLK PCout(7)
#define SSI_DO PCin(8)
#define SSI_CSN PCout(6)

extern u16 MotorPWM;
extern u16 SSI_LOC ;
void SSI_init(void);
void get_SSI(void);
void Angle_Cal(void);

#endif
