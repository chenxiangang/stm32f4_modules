#ifndef _DMA_UART_H_
#define _DMA_UART_H_
#include "sys.h"

#define Photo_Mode 1
#define CCD_Mode 2
#define BX_Mode 3

u8 UART2_DMA_Init(u8** argv, int size, u8 mode);

#endif
