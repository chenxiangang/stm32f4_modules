#ifndef _BTN7971_H_
#define _BTN7971_H_

#include "sys.h"

/*********************
全部使用宏定义，请按照实际需求改
尽量用一个定时器吧
默认输出频率10K
建议使用前用示波器检查
*********************/




//对于电机输出口的宏定义
//记得按照实际接口修改
#define  BTN7971_EN1_IN1_RCC_GPIO  (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN1_IN2_RCC_GPIO  (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN2_IN1_RCC_GPIO  (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN2_IN2_RCC_GPIO  (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN1_IN1      (PAout(0))            //TIM5_CH1
#define  BTN7971_EN1_IN2      (PAout(1))            //TIM5_CH2
#define	 BTN7971_EN2_IN1			(PAout(2))            //TIM5_CH3
#define  BTN7971_EN2_IN2			(PAout(3))						//TIM5_CH4


//
#define  BTN7971_TIM_ch1      GPIO_Pin_0
#define  BTN7971_TIM_ch2      GPIO_Pin_1
#define  BTN7971_TIM_ch3      GPIO_Pin_2
#define  BTN7971_TIM_ch4      GPIO_Pin_3

//输入GPIO
#define  BTN7971_EN1_IN1_GPIO      (GPIOA)            //GPIOA
#define  BTN7971_EN1_IN2_GPIO      (GPIOA)            //GPIOA
#define	 BTN7971_EN2_IN1_GPIO			 (GPIOA)            //GPIOA
#define  BTN7971_EN2_IN2_GPIO			 (GPIOA)            //GPIOA

#define  BTN7971_EN1_IN1_GPIOPINSOURCE (GPIO_PinSource0)  
#define  BTN7971_EN1_IN2_GPIOPINSOURCE (GPIO_PinSource1)
#define  BTN7971_EN2_IN1_GPIOPINSOURCE (GPIO_PinSource2)
#define  BTN7971_EN2_IN2_GPIOPINSOURCE (GPIO_PinSource3)  

//对于使能口的定义
//也可以直接接高电平
//按实际引脚改
#define	 BTN7971_EN1_RCC_GPIO   (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN2_RCC_GPIO		(RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN1						(PAout(4))						//使能1
#define  BTN7971_EN2						(PAout(5))						//使能2

//使能端的GPIO引脚
#define BTN7971_EN1_PIN   GPIO_Pin_4
#define BTN7971_EN2_PIN   GPIO_Pin_5

//TIM_PWM
#define BTN7971_TIM_RCC (RCC_APB1Periph_TIM5)
//
#define BTN7971_TIM    (TIM5)
//TIM AF
#define BTN7971_TIM_AF (GPIO_AF_TIM5)


// 84M/(PSC+1)/(ARR+1)   10K
//预分频系数
#define BTN7971_PSC   (84-1)
//重装载值
#define BTN7971_ARR		(100-1)
//
#define MAX_SPEED   BTN7971_ARR    //最大速度


void BTN7971_EN_Init(int channel);  //初始化EN口，1是EN1，2是EN2，3是全部   默认使能口高电平
void BTN7971_EN1_PWM_Init(void);  //初始化EN1的两个PWM输入    默认输出0
void BTN7971_EN2_PWM_Init(void);  //初始化EN2的两个输入				默认输出0
void BTN7971_Init(void);          //只是上面3个函数的包含	
void contrl_speed(int speed,int ch);  //speed为速度输入,  ch通道选择，1位EN1，2为EN2
#endif
