#ifndef _NRF_H_
#define _NRF_H_

#include "sys.h"
#include "OV7620.h"

#define NRF_RST PDin(2)
#define USART2_REC_LEN  	200 
#define Photo_Mode 1
#define CCD_Mode 2
#define BX_Mode 3

void NRF_Init(int bound);
int NRF_Send(u8 *buf, int len,u8 mode);
u8 NRF_Send_One(u8 data);
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr);
void NRF_DMA_Send(int size);
int NRF_DMA(u8 argv[][CAMERA_W], int size, u8 mode);
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
#endif
