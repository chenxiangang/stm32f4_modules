#ifndef __SPEED_CAP_H
#define __SPEED_CAP_H
#include "sys.h"
/***********************************************************
Copyright (C) ,2013-2023,YFRobot
Author: aosini   Version:1.0     Date:2014/03/04
BaseOn:YFRobot CoreSTM32 V1.0
Description: YFRobot 371电机速度检测( 编码器)
          红黑线为编码器的正负极，不可接反，接3.3V
      编码器接口模式 ，速度检测与方向检测
      PA0(TIM2_CH1),接绿线；PA1(TIM2_CH2)，接白线。
All rights reserved
***********************************************************/
void TIM4_Encoder_Init(void);

int TIM4_Encoder_Read(void);

#endif
