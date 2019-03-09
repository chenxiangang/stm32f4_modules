/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 11:32:37
 * @LastEditTime: 2019-03-09 11:35:13
 */
#include "motor.h"
#include "sys.h"
/***********************************************************
Copyright (C) ,2013-2023,YFRobot
Author: aosini   Version:1.0     Date:2014/03/04
BaseOn:YFRobot CoreSTM32 V1.0
Description: YFRobot L298N V1.1 驱动代码
             电机驱动VIN、GND接7.2V
             IN1~2接PC0~1；方向控制端口
                         ENA接PA6:速度控制
                         电机两根黄线接在298 M1处
All rights reserved
***********************************************************/

// LED IO 初始化 端口PD.2  运行指示灯
// void LED_Init(void)
//{
//	RCC->APB2ENR|=1<<5;      //使能PORTD时钟
//	GPIOD->CRL&=0XFFFFF0FF;
//	GPIOD->CRL|=0X00000300;  //PD.2推挽输出
//	GPIOD->ODR|=1<<2;        //PD.2输出高
//}
//电机旋转方向控制信号端口初始化
// PC1~0推挽输出，输出高
/**
void M_Init(void) {

  RCC->APB2ENR |= 1 << 4; //使能PORTC时钟
  GPIOC->CRL &= 0XFFFFFF00;
  GPIOC->CRL |= 0X00000033; // PC1~0推挽输出
  GPIOC->ODR |= 0XF << 0;   // PC1~0输出高电平
}
**/
void Motor_Speed_Control(s16 motorSpeed) {
  s16 speed = 0;

  if (motorSpeed > 100)
    speed = 900;
  else if (motorSpeed < -100)
    speed = -900;
  else
    speed = motorSpeed;
  if (speed == 0) {

  } else if (speed > 0) {
    TIM4->CCR1 = speed * 5 + 400;
  } else {

    TIM4->CCR1 = -speed * 5 + 400;
  }
}

//定时器TIM3，PWM输出初始化,CH1（PA6)
// arr：自动重装值
// psc：时钟预分频数
//设置自动重装值为900，那么PWM频率=72000/900=8Khz
////见STM32参考手册，14.3.9PWM模式。
/*void TIM3_PWM_Init(u16 arr,u16 psc) //arr设定计数器自动重装值
                                                                                                                                                //psc预分频器不分频,psc=0
{
        RCC->APB1ENR|=1<<1;       //TIM3时钟使能
                        

        GPIOA->CRL&=0XF0FFFFFF;//PA6输出
        GPIOA->CRL|=0X0B000000;//复用功能输出
        GPIOA->ODR|=1<<6;//PA6上拉

        TIM3->ARR=arr;//设定计数器自动重装值
        TIM3->PSC=psc;//预分频器不分频
        

        TIM3->CCMR1|=6<<4;  //CH1 PWM1模式	高电平有效
        TIM3->CCMR1|=1<<3; //CH1预装载使能

        TIM3->CCER|=1<<0;   //OC1 输出使能

        TIM3->CR1=0x0080;   //ARPE使能
        TIM3->CR1|=0x01;    //使能定时器3
}*/

//电机方向与速度控制，速度调节范围为-100~+100
//大于0时，正转，小于0时，反转
// 占空比低于0.4时电机不转
//（占空比是指高电平在一个周期之内所占的时间比率）
// TIM3->CCR1的设定范围为0~900（因为arr=900）
//见STM32参考手册，14.3.9PWM模式。
