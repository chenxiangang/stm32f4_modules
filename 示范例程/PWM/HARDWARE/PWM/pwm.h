#ifndef _PWM_H_
#define _PWM_H_
#include "sys.h"
extern int CRR;
extern int deadtime;
void pwm_init(uint16_t arr,uint16_t psc);
#endif
