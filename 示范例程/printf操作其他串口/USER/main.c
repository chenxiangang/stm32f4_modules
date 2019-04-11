#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "DMA_UART.h"
#include "uart_p.h"

//ALIENTEK 探索者STM32F407开发板 实验0
//STM32F4工程模板-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

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


