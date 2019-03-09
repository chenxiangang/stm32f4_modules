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
Description: YFRobot L298N V1.1 ��������
             �������VIN��GND��7.2V
             IN1~2��PC0~1��������ƶ˿�
                         ENA��PA6:�ٶȿ���
                         ����������߽���298 M1��
All rights reserved
***********************************************************/

//#define M_1 PCout(0)  //���������ƶ˿�
//#define M_2 PCout(1)

//#define LED PDout(2)  // ����ָʾ��

// void LED_Init(void);//��ʼ��ָʾ�ƶ˿�
// void M_Init(void);	//��ʼ��������з�����ƶ˿�
// void TIM3_PWM_Init(u16 arr,u16 psc); //��ʱ��TIM3��PWM�����ʼ��,CH1��PA6��
// arr�趨�������Զ���װֵ��pscԤ��Ƶ������Ƶ,psc=0
void Motor_Speed_Control(
    s16 motorSpeed); //����������ٶȿ��ƣ��ٶȵ��ڷ�ΧΪ-100~+100
#endif
