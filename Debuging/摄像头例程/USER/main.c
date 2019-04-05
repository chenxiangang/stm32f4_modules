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
#include "OLED.h"
#include "camera_deal.h"

int limit = 0;

int main(void)
{
	//uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	OLED_Init();
	OV7620_Init();
	
	limit = 10;
  while(1){
		 Threshold_Camera(limit);
		if(ImgControl.ImgOk == true)
     {
        ImgControl.ImgDealOk = true;
     }
		 if(ImgControl.ImgDealOk)
			Camera_OLED(0,0);
	}
}


