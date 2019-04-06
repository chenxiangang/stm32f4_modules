#ifndef __MYNVIC_H
#define __MYNVIC_H
#include "sys.h"

/*************************** system nvic order *************************/
#define Enable_Interrupts() __enable_irq() //open  all interrupts
#define Disable_Interrupts() __disable_irq() //close all interrupts

void My_NVIC_Init(void);

#endif
