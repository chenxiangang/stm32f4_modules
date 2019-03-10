/*------------------------
 ��TIM2�ж���ִ�п��Ʋ���
 ��Ҫ��ʼ��TIM2_Int_Init(arr,psc);
 �ѿ������ݷŵ��жϺ�������ȥ
------------------------*/
#include "timer.h"

//-------------------�����Ҫ��ͷ�ļ�---------------
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "control.h"
#include "delay.h"
#include "pid.h"
#include "usart.h"
//-------------------------------------------------

//------------------���õ��ⲿ����-----------------
extern double Angle;
extern PID pid1;
extern PID pid2;
extern int speed;
extern int last_speed;
extern double pwmduty;
extern u16 realpwmduty;
extern double dutychange2;
extern double rout;
extern u16 uccr1;
extern u16 nccr1;
extern PID sPID;
//--------------------------------------------------

void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //NVIC������mynvic.c��

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE); //ʹ��TIMx����
}

void TIM2_IRQHandler(void) //TIM2�ж�
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        swing();
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
