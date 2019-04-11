#ifndef _DMA_UART_H_
#define _DMA_UART_H_
#include "sys.h"

#define Photo_Mode 1
#define CCD_Mode 2
#define BX_Mode 3
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
u8 UART2_DMA_Init(u8** argv, int size, u8 mode);
void uart2_init(u32 bound);
#endif
