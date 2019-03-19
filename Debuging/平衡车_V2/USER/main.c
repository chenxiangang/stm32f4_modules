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
���Թܣ� PD0 PD1 PD2 PD3 
ENCODER1��   PD12 PD13  (TIM4)
ENCODER2�� ����
TB6612��PWM��PA6 PA7(Ŀǰ��LED��ͻ)   (TIM3)          AIN1 AIN2 BIN1 BIN2   PA0 PA1 PA2 PA3    STBY�ӵ�Դ3.3V
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
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	YL_70_Init();         //��ʼ�����Թ�
	TIM3_Init(100,7199);   //��ȡ���������ݣ�����pid����
	Encoder_TIM4_Init();  //��ʼ�����������A
	Encoder_TIM2_Init();
	uart_init(115200);    //��ʼ������1�����ڷ������ݵ���λ��
	usart3_init(115200);  //������ȡ�����ǵ�����
	My_NVIC_Init();
	
	
	
	
	while(1)
	{
	}
}





