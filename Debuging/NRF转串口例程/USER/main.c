#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "nrf.h"
#include "OV7620.h"
//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
	int i=0;
int main(void)
{

	delay_init(168);
	NRF_Init(460800);
  while(1){
		i++;
		NRF_DMA(Image,CAMERA_H*CAMERA_W,Photo_Mode);
	}
}



