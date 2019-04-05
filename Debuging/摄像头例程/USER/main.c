/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 13:00:53
 * @LastEditTime: 2019-04-05 13:45:16
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "OV7620.h"

int main(void)
{
	uart_init(115200);
	delay_init(168);
	OV7620_Init();
  while(1){
		delay_ms(500);
	}
}


