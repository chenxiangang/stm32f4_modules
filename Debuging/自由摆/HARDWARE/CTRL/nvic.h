#ifndef __NVIC_H
#define __NVIC_H
#include "sysincludes.h"

/*************************** system nvic order *************************/
#define Enable_Interrupts()   __enable_irq()     //open  all interrupts
#define Disable_Interrupts()  __disable_irq()    //close all interrupts


void Nvic_Init(void);

#endif
