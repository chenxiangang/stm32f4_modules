#ifndef __SPEED_CAP_H
#define __SPEED_CAP_H
#include "sys.h"
/***********************************************************
Copyright (C) ,2013-2023,YFRobot
Author: aosini   Version:1.0     Date:2014/03/04
BaseOn:YFRobot CoreSTM32 V1.0
Description: YFRobot 371����ٶȼ��( ������)
          �����Ϊ�������������������ɽӷ�����3.3V
      �������ӿ�ģʽ ���ٶȼ���뷽����
      PA0(TIM2_CH1),�����ߣ�PA1(TIM2_CH2)���Ӱ��ߡ�
All rights reserved
***********************************************************/
void TIM4_Encoder_Init(void);

int TIM4_Encoder_Read(void);

#endif
