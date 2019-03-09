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
Description: YFRobot 371电机速度检测( 编码器)
      红黑线为编码器的正负极，不可接反，接3.3V
      编码器接口模式 ， 速度检测与方向检测
      PA0(TIM2_CH1),接绿线；PA1(TIM2_CH2)，接白线。
All rights reserved
***********************************************************/
// TIM2_Encoder_Init,Tim2_CH1(PA0);Tim2_CH2(PA1)
// arr：自动重装值 0XFFFF
// psc：时钟预分频数	，不分频
//见STM32中文手册 14.3.12编码器接口模式
void TIM4_Encoder_Init(void) //正交解码
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //TIM4时钟

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); //PD12
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); //PD13

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //使能D12、D13引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			 // 浮空输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//初始化TIM4  /*TIM4编码器模式初始化*/

	TIM_DeInit(TIM4); //复位

	TIM_TimeBaseStructure.TIM_Period = 0xffff;					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 0;					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//?还是0？ //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//编码配置                        编码模式
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); // TIM_ICPolarity_Rising上升沿捕获
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6; //比较滤波器
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	// TIM_ARRPreloadConfig(TIM3, ENABLE);
	// Clear all pending interrupts
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	// TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //使能中断
	// Reset counter
	TIM4->CNT = 0;

	TIM_Cmd(TIM4, ENABLE); //使能定时器3

	//  RCC->APB1ENR|=1<<0;       //TIM4时钟使能
	//  RCC->APB2ENR|=1<<2;       //使能PORTA时钟
	//  GPIOA->CRL&=0XFFFFFF00;   //PA0、PA1 清除之前设置
	//  GPIOA->CRL|=0X00000044;   //PA0、PA1 浮空输入

	//	TIM4->ARR=arr;            //设定计数器自动重装值
	//	TIM4->PSC=psc;            //预分频器
	//	TIM4->CCMR1 |= 1<<0;	  //输入模式，IC1FP1映射到TI1上
	//	TIM4->CCMR1 |= 1<<8;	  //输入模式，IC2FP2映射到TI2上
	//	TIM4->CCER |= 0<<1;		  //IC1不反向
	//	TIM4->CCER |= 0<<5; 	  //IC2不反向
	//	TIM4->SMCR |= 3<<0;		  //所用输入均在上升沿或下降沿有效
	//  TIM4->CR1 |= 1<<0;		  //使能计数器
}

int TIM4_Encoder_Read(void)
{
	return (int)(s16)TIM4->CNT; //数据类型转换
} //记录边沿变化次数（几个栅格被记录4次）
