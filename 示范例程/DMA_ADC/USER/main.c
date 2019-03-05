#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"

//dma_adc�������С
#define adc_get_buffer_size   1
//dma_adc������adc��ȡ
u16 adc_get_v[adc_get_buffer_size];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	uart_init(115200);
	delay_init(168);
	dma_adc_init((u32)adc_get_v,adc_get_buffer_size);  //��ʼ��
  while(1){
    printf("%d\r\n",adc_get_v[0]);
		delay_ms(200);
	}
}




