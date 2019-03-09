/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 11:32:37
 * @LastEditTime: 2019-03-09 11:35:14
 */
#ifndef __MOTOR_H
#define __MOTOR_H
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

//#define M_1 PCout(0)  //电机方向控制端口
//#define M_2 PCout(1)

//#define LED PDout(2)  // 运行指示灯

// void LED_Init(void);//初始化指示灯端口
// void M_Init(void);	//初始化电机运行方向控制端口
// void TIM3_PWM_Init(u16 arr,u16 psc); //定时器TIM3，PWM输出初始化,CH1（PA6）
// arr设定计数器自动重装值，psc预分频器不分频,psc=0
void Motor_Speed_Control(
    s16 motorSpeed); //电机方向与速度控制，速度调节范围为-100~+100
#endif
