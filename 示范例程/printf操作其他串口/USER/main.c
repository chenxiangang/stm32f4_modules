#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "DMA_UART.h"
#include "uart_p.h"

//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
	uart_init(115200);
	delay_init(168);
	uart2_init(115200);
  while(1){
	u2_printf("hello\r\n");
		delay_ms(100);
	}
}


