#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "JY901_uart.h"
#include "include.h"

FLAG_Typedef     flag;



int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	usart3_init(115200);
	
	while(1)
	{
		printf("%f %f %f\r\n",pitch,roll,yaw);
		delay_ms(10);
				   
	}    
}

