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
#include "FreeCars_uart.h"


u8 Image_Bin[CAMERA_H][CAMERA_W]={0};

int main(void)
{
	u32 i,j;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	OLED_Init();
	OV7620_Init();
	uart2_init(115200);
  while(1){
//		 Threshold_Camera(10);
//		if(ImgControl.ImgOk == true)
//     {
//        ImgControl.ImgDealOk = true;
//     }
//		 if(ImgControl.ImgDealOk)
//			Camera_OLED(0,0);
//					  for(i = 0; i < CAMERA_H; i++)
//				{ 
//					for(j = 0; j < CAMERA_W; j++)
//					{
//						//Image[][]是存放原始图像数据的数组，Image_Bin是存放二值化之后图像数据的数组
//						Image_Bin[i][j]=Image[i][j];
//						
//						//二值化  白:0xff  黑:0x00
//						if(Image_Bin[i][j]>0X46) Image_Bin[i][j] = 0xff;    //黑色<113(0x71)    白色>113  白色<231(0xe7)
//						else	 Image_Bin[i][j] = 0X00;                      //激光>231
////						if(lie<100&&hang<100) Image_Bin[hang][lie]=0x00; 
////						Image_Bin[60][60]=0x00;
////						if(l_lie<120&&l_hang<120) Image[l_hang][l_lie]=0x00; 
//					}
//				}
		SendOV7620();
				delay_ms(300);
	}
}


