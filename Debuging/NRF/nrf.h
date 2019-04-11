#ifndef _NRF_H_
#define _NRF_H_

#include "sys.h"

#define NRF_RST PDin(2)
#define USART2_REC_LEN  	200 
#define Photo_Mode 1
#define CCD_Mode 2
#define BX_Mode 3

void NRF_Init(int bound);
int NRF_Send(u8 *buf, int len,u8 mode);
u8 NRF_Send_One(u8 data);
#endif