#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"

//dma_adc的数组大小
#define adc_get_buffer_size   1
//dma_adc方法的adc读取
u16 adc_get_v[adc_get_buffer_size];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);
	delay_init(168);
	dma_adc_init((u32)adc_get_v,adc_get_buffer_size);  //初始化
  while(1){
    printf("%d\r\n",adc_get_v[0]);
		delay_ms(200);
	}
}




