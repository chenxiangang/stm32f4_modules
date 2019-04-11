#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "nrf.h"
//ALIENTEK 探索者STM32F407开发板 实验0
//STM32F4工程模板-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{
	delay_init(168);
	NRF_Init(460800);
  while(1){
		NRF_Send_One('h');
		NRF_Send_One('\r');
		NRF_Send_One('\n');
		delay_ms(1);
	}
}



