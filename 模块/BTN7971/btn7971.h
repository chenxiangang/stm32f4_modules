#ifndef _BTN7971_H_
#define _BTN7971_H_

#include "sys.h"

/*********************
ȫ��ʹ�ú궨�壬�밴��ʵ�������
������һ����ʱ����
Ĭ�����Ƶ��10K
����ʹ��ǰ��ʾ�������
*********************/




//���ڵ������ڵĺ궨��
//�ǵð���ʵ�ʽӿ��޸�
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

//����GPIO
#define  BTN7971_EN1_IN1_GPIO      (GPIOA)            //GPIOA
#define  BTN7971_EN1_IN2_GPIO      (GPIOA)            //GPIOA
#define	 BTN7971_EN2_IN1_GPIO			 (GPIOA)            //GPIOA
#define  BTN7971_EN2_IN2_GPIO			 (GPIOA)            //GPIOA

#define  BTN7971_EN1_IN1_GPIOPINSOURCE (GPIO_PinSource0)  
#define  BTN7971_EN1_IN2_GPIOPINSOURCE (GPIO_PinSource1)
#define  BTN7971_EN2_IN1_GPIOPINSOURCE (GPIO_PinSource2)
#define  BTN7971_EN2_IN2_GPIOPINSOURCE (GPIO_PinSource3)  

//����ʹ�ܿڵĶ���
//Ҳ����ֱ�ӽӸߵ�ƽ
//��ʵ�����Ÿ�
#define	 BTN7971_EN1_RCC_GPIO   (RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN2_RCC_GPIO		(RCC_AHB1Periph_GPIOA)
#define  BTN7971_EN1						(PAout(4))						//ʹ��1
#define  BTN7971_EN2						(PAout(5))						//ʹ��2

//ʹ�ܶ˵�GPIO����
#define BTN7971_EN1_PIN   GPIO_Pin_4
#define BTN7971_EN2_PIN   GPIO_Pin_5

//TIM_PWM
#define BTN7971_TIM_RCC (RCC_APB1Periph_TIM5)
//
#define BTN7971_TIM    (TIM5)
//TIM AF
#define BTN7971_TIM_AF (GPIO_AF_TIM5)


// 84M/(PSC+1)/(ARR+1)   10K
//Ԥ��Ƶϵ��
#define BTN7971_PSC   (84-1)
//��װ��ֵ
#define BTN7971_ARR		(100-1)
//
#define MAX_SPEED   BTN7971_ARR    //����ٶ�


void BTN7971_EN_Init(int channel);  //��ʼ��EN�ڣ�1��EN1��2��EN2��3��ȫ��   Ĭ��ʹ�ܿڸߵ�ƽ
void BTN7971_EN1_PWM_Init(void);  //��ʼ��EN1������PWM����    Ĭ�����0
void BTN7971_EN2_PWM_Init(void);  //��ʼ��EN2����������				Ĭ�����0
void BTN7971_Init(void);          //ֻ������3�������İ���	
void contrl_speed(int speed,int ch);  //speedΪ�ٶ�����,  chͨ��ѡ��1λEN1��2ΪEN2
#endif
