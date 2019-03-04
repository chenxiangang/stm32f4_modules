#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "JY901_uart.h"
#include "include.h"

FLAG_Typedef     flag;



int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usart3_init(115200);
	
	while(1)
	{
		printf("%f %f %f\r\n",pitch,roll,yaw);
		delay_ms(10);
				   
	}    
}

