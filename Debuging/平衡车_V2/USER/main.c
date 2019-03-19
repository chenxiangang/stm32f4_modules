#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "YL70.h"
#include "encoder.h"
#include "JY901_uart.h"
#include "include.h"
#include "FreeCars_uart.h"
#include "mynvic.h"
#include "control.h"

/*
光电对管： PD0 PD1 PD2 PD3 
ENCODER1：   PD12 PD13  (TIM4)
ENCODER2： 待定
TB6612：PWM：PA6 PA7(目前与LED冲突)   (TIM3)          AIN1 AIN2 BIN1 BIN2   PA0 PA1 PA2 PA3    STBY接电源3.3V
JY901:     USART3  TX PD8    RX PD9     
OLED:   CS  PD3 
        RST PD4
				DC  PD5
				D0  PD6
				D1  PD7

*/

FLAG_Typedef flag;

int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	YL_70_Init();         //初始化光电对管
	TIM3_Init(100,7199);   //读取传感器数据，进行pid控制
	Encoder_TIM4_Init();  //初始化电机编码器A
	Encoder_TIM2_Init();
	uart_init(115200);    //初始化串口1，用于发送数据到上位机
	usart3_init(115200);  //用来读取陀螺仪的数据
	My_NVIC_Init();
	
	
	
	
	while(1)
	{
	}
}





