#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "JY901_uart.h"
#include "include.h"
#include "FreeCars_uart.h"

FLAG_Typedef     flag;

int a = 1;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	usart3_init(115200);
	OLED_Init();
	LED_Init();					//初始化LED 
	while(1) 
	{
		push(0,a);
		uSendOnePage();
		printf("hello world");
		delay_ms(100);
		LED0=!LED0;
	}
}
