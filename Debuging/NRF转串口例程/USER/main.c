#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "nrf.h"
//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
	delay_init(168);
	NRF_Init(460800);
  while(1){
		NRF_Send_One('h');
		NRF_Send_One('\r');
		NRF_Send_One('\n');
		delay_ms(1);
	}
}



