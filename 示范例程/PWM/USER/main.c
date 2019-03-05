#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"

int arr=500;
int psc=84;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);
	delay_init(164);
	pwm_init(arr-1,psc-1);  //168M/(psc+1)/(arr+1)
  while(1)
	{
    
	}
}




