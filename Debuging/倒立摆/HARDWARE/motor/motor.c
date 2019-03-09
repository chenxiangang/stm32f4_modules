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
Description: YFRobot L298N V1.1 ��������
             �������VIN��GND��7.2V
             IN1~2��PC0~1��������ƶ˿�
                         ENA��PA6:�ٶȿ���
                         ����������߽���298 M1��
All rights reserved
***********************************************************/

// LED IO ��ʼ�� �˿�PD.2  ����ָʾ��
// void LED_Init(void)
//{
//	RCC->APB2ENR|=1<<5;      //ʹ��PORTDʱ��
//	GPIOD->CRL&=0XFFFFF0FF;
//	GPIOD->CRL|=0X00000300;  //PD.2�������
//	GPIOD->ODR|=1<<2;        //PD.2�����
//}
//�����ת��������źŶ˿ڳ�ʼ��
// PC1~0��������������
/**
void M_Init(void) {

  RCC->APB2ENR |= 1 << 4; //ʹ��PORTCʱ��
  GPIOC->CRL &= 0XFFFFFF00;
  GPIOC->CRL |= 0X00000033; // PC1~0�������
  GPIOC->ODR |= 0XF << 0;   // PC1~0����ߵ�ƽ
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

//��ʱ��TIM3��PWM�����ʼ��,CH1��PA6)
// arr���Զ���װֵ
// psc��ʱ��Ԥ��Ƶ��
//�����Զ���װֵΪ900����ôPWMƵ��=72000/900=8Khz
////��STM32�ο��ֲᣬ14.3.9PWMģʽ��
/*void TIM3_PWM_Init(u16 arr,u16 psc) //arr�趨�������Զ���װֵ
                                                                                                                                                //pscԤ��Ƶ������Ƶ,psc=0
{
        RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��
                        

        GPIOA->CRL&=0XF0FFFFFF;//PA6���
        GPIOA->CRL|=0X0B000000;//���ù������
        GPIOA->ODR|=1<<6;//PA6����

        TIM3->ARR=arr;//�趨�������Զ���װֵ
        TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
        

        TIM3->CCMR1|=6<<4;  //CH1 PWM1ģʽ	�ߵ�ƽ��Ч
        TIM3->CCMR1|=1<<3; //CH1Ԥװ��ʹ��

        TIM3->CCER|=1<<0;   //OC1 ���ʹ��

        TIM3->CR1=0x0080;   //ARPEʹ��
        TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
}*/

//����������ٶȿ��ƣ��ٶȵ��ڷ�ΧΪ-100~+100
//����0ʱ����ת��С��0ʱ����ת
// ռ�ձȵ���0.4ʱ�����ת
//��ռ�ձ���ָ�ߵ�ƽ��һ������֮����ռ��ʱ����ʣ�
// TIM3->CCR1���趨��ΧΪ0~900����Ϊarr=900��
//��STM32�ο��ֲᣬ14.3.9PWMģʽ��
