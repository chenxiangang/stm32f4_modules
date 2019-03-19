#include "math.h"
#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "btn7971.h"
#include "oled.h"
#include "timercap.h"
#include "usart.h"


double encoder_angle;
double pwmduty;
extern PID pid1;
extern PID pid2;

//void motorback()
//{
//	encoder_angle = Read_Encoder(); //��ȡĿǰ�������Ƕ�

//	if (pid1.SetPoint - encoder_angle > 180 || pid1.SetPoint - encoder_angle < -180) //��ԭ������Ȧ����ʱ��ȫ�ٷ�ת
//	{
//		if (encoder_angle > 180)
//			contrl_speed(1080, 1);
//		else
//			contrl_speed(-1080, 1);
//	}

//	else //��ԭ������Ȧ����ʱ����ʼ���٣���Ϊ������Ҫ������Ҫ������
//	{
//		pwmduty = PIDCalc(&pid1, encoder_angle); //����pid����

//		if (pwmduty > 0)
//			contrl_speed(pwmduty + 80, 1); //��2100��arr�£���80������
//		else
//			contrl_speed(pwmduty - 80, 1);
//	}
//}

void SetAngle(double Setangle)
{
	pid1.SetPoint = Setangle;

	pwmduty = PIDCalc(&pid1, Read_Encoder());
	if (pwmduty > 0)
		contrl_speed(pwmduty + 110, 1); //��2100��arr�£���80������
	else
		contrl_speed(pwmduty - 110, 1);
}

//��ʱ��3��ʼ��
//arr���Զ���װ�س�ֵ
//psc��Ԥ��Ƶϵ��
//���ʱ��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///������ʱ��3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//�Զ���װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //ʱ����ʼ��

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //����TIM3�����ж�
	TIM_Cmd(TIM3, ENABLE);					   //ʹ��TIM3

	// NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //TIM3�ж�
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //�����ȼ�
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //�����ȼ�
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

//TIM3�жϴ���
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //�ж�
	{
		if(out_mode)  //�Ƕ�
		{
			//pid1.SetPoint=out_msg;  //�����趨ֵ
			SetAngle(out_msg);
		}
		else
		{
     pid2.SetPoint = pid2.SetPoint + 0.006 * out_msg;
		

	  pwmduty = PIDCalc(&pid2, Read_Encoder());
	  if (pwmduty > 0)
		  contrl_speed(pwmduty + 110, 1); //��2100��arr�£���80������
	  else
		  contrl_speed(pwmduty - 110, 1);
		}
		//TO DO
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //�����־λ
}

void ClearAll(void)
{
	    TIM2CH4_CAPTURE_STA &= 0x00; //����
      angle = 0;                   //����
      TIM4->CNT = 0x7FFF;          //
			pid1.SetPoint = 0;
			pid2.SetPoint = 0;
}
