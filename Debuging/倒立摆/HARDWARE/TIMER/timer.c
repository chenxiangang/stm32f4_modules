/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 12:12:25
 * @LastEditTime: 2019-03-09 12:38:59
 */
#include "timer.h"
#include "control.h"
#include "led.h"
#include "usart.h"
#include "SSI4096.h"
#include "delay.h"

extern int ActionFlag;
extern double Angle;

void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM3, ENABLE); //ʹ��TIMx����
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void) //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIMx���жϴ�����λ:TIM �ж�Դ
        LED1 = !LED1;
    }
}

void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM2, ENABLE); //ʹ��TIMx����
}

void TIM2_IRQHandler(void) //TIM2�ж�
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        Angle_Cal();
        if (Angle > sPID.SetPoint - 35 && Angle < sPID.SetPoint + 35)
            ActionFlag = 2;
        else
        {
            ActionFlag = 0;
            sPID.SumError = 0;
            motor_action(0);
            delay_ms(1000);
        }
        //			else if(ActionFlag != 1)
        //			{
        //				motor_action(0);
        //				while(!(Angle < 10 && Angle > 350))  //�����ȶ��������������
        //				{
        //					delay_ms(5000);
        //					Angle_Cal();
        //				}
        //				ActionFlag = 1;
        //			}

        switch (ActionFlag)
        {
        case 1:
            Start_Swing();
            break; //���
        case 2:
            Revert();
            break; //����
        case 0:
            motor_action(0); //ֹͣ
            sPID.SumError = 0;
            break;
        }
    }
    uccr1 = nccr1;
    nccr1 = TIM4->CCR1; //�е�����
    speed = nccr1 - uccr1;
    if (speed < 0)
        speed = -speed;
    printf("%d\r\n", speed);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//TIM3 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ�ܶ�ʱ��3ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); //PB5

    //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM3 Channel2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //����Tָ���Ĳ�����ʼ������TIM3 OC2

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE); //ʹ��TIM3
}
