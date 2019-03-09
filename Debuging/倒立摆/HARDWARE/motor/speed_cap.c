/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 11:32:37
 * @LastEditTime: 2019-03-09 11:50:22
 */
#include "speed_cap.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
/***********************************************************
Copyright (C) ,2013-2023,YFRobot
Author: aosini   Version:1.0     Date:2014/03/04
BaseOn:YFRobot CoreSTM32 V1.0
Description: YFRobot 371����ٶȼ��( ������)
      �����Ϊ�������������������ɽӷ�����3.3V
      �������ӿ�ģʽ �� �ٶȼ���뷽����
      PA0(TIM2_CH1),�����ߣ�PA1(TIM2_CH2)���Ӱ��ߡ�
All rights reserved
***********************************************************/
// TIM2_Encoder_Init,Tim2_CH1(PA0);Tim2_CH2(PA1)
// arr���Զ���װֵ 0XFFFF
// psc��ʱ��Ԥ��Ƶ��	������Ƶ
//��STM32�����ֲ� 14.3.12�������ӿ�ģʽ
void TIM4_Encoder_Init(void) //��������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //TIM4ʱ��

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); //PD12
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); //PD13

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //ʹ��D12��D13����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			 // ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//��ʼ��TIM4  /*TIM4������ģʽ��ʼ��*/

	TIM_DeInit(TIM4); //��λ

	TIM_TimeBaseStructure.TIM_Period = 0xffff;					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//?����0�� //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��������                        ����ģʽ
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); // TIM_ICPolarity_Rising�����ز���
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6; //�Ƚ��˲���
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	// TIM_ARRPreloadConfig(TIM3, ENABLE);
	// Clear all pending interrupts
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	// TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //ʹ���ж�
	// Reset counter
	TIM4->CNT = 0;

	TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��3

	//  RCC->APB1ENR|=1<<0;       //TIM4ʱ��ʹ��
	//  RCC->APB2ENR|=1<<2;       //ʹ��PORTAʱ��
	//  GPIOA->CRL&=0XFFFFFF00;   //PA0��PA1 ���֮ǰ����
	//  GPIOA->CRL|=0X00000044;   //PA0��PA1 ��������

	//	TIM4->ARR=arr;            //�趨�������Զ���װֵ
	//	TIM4->PSC=psc;            //Ԥ��Ƶ��
	//	TIM4->CCMR1 |= 1<<0;	  //����ģʽ��IC1FP1ӳ�䵽TI1��
	//	TIM4->CCMR1 |= 1<<8;	  //����ģʽ��IC2FP2ӳ�䵽TI2��
	//	TIM4->CCER |= 0<<1;		  //IC1������
	//	TIM4->CCER |= 0<<5; 	  //IC2������
	//	TIM4->SMCR |= 3<<0;		  //����������������ػ��½�����Ч
	//  TIM4->CR1 |= 1<<0;		  //ʹ�ܼ�����
}

int TIM4_Encoder_Read(void)
{
	return (int)(s16)TIM4->CNT; //��������ת��
} //��¼���ر仯����������դ�񱻼�¼4�Σ�
