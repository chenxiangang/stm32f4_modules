/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-04 23:35:00
 * @LastEditTime: 2019-03-05 13:16:23
 */
#ifndef _PWM_H_
#define _PWM_H_
#include "sys.h"
extern int CRR;
extern int deadtime;
void pwm_init(uint16_t arr,uint16_t psc);
#endif
