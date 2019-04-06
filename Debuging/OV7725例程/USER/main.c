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
#include "OLED.h"
#include "OV7725.h"



int main(void)
{
	//uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	OLED_Init();
	OV7725_Init();
	

  while(1){		
		OLED_Show(0,0);
		}
}
