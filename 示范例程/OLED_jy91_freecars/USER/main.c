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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	usart3_init(115200);
	OLED_Init();
	LED_Init();					//��ʼ��LED 
	while(1) 
	{
		push(0,a);
		uSendOnePage();
		printf("hello world");
		delay_ms(100);
		LED0=!LED0;
	}
}
