/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-18 17:27:19
 * @LastEditTime: 2019-03-18 18:35:10
 */
#include "pwm.h"

//上位机TIM2初始化PWM
void TIM2_PWM_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;               //GPIO初始化结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; //时基初始化结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;             //定时器输出结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //开启GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;          //PB11 TIM2_CH4
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速
    GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化GPIOB

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2); //PB11 复用为TIM2

    //84000000/84/50000=20Hz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //1分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period = (50000 - 1);             //arr 自动重装载初值
    TIM_TimeBaseInitStructure.TIM_Prescaler = (84 - 1);             //psc分频系数 84M分频
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //PWM模式1 低于CCR为有效极性
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //有效极性为高
    TIM_OCInitStructure.TIM_Pulse = 0;                            //初始CRR为0  即开始不转
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                      //CH4

    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
    TIM_ARRPreloadConfig(TIM2, ENABLE);               //允许工作时改ARR  更新到来才能改
    TIM_Cmd(TIM2, ENABLE);                            //开启TIM2
}
