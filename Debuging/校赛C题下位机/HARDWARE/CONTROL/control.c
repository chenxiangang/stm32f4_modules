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
//	encoder_angle = Read_Encoder(); //读取目前编码器角度

//	if (pid1.SetPoint - encoder_angle > 180 || pid1.SetPoint - encoder_angle < -180) //与原点相差半圈以上时，全速反转
//	{
//		if (encoder_angle > 180)
//			contrl_speed(1080, 1);
//		else
//			contrl_speed(-1080, 1);
//	}

//	else //与原点相差半圈以内时，开始减速，因为精度有要求，所以要防超调
//	{
//		pwmduty = PIDCalc(&pid1, encoder_angle); //进行pid运算

//		if (pwmduty > 0)
//			contrl_speed(pwmduty + 80, 1); //在2100的arr下，有80的死区
//		else
//			contrl_speed(pwmduty - 80, 1);
//	}
//}

void SetAngle(double Setangle)
{
	pid1.SetPoint = Setangle;

	pwmduty = PIDCalc(&pid1, Read_Encoder());
	if (pwmduty > 0)
		contrl_speed(pwmduty + 110, 1); //在2100的arr下，有80的死区
	else
		contrl_speed(pwmduty - 110, 1);
}

//定时器3初始化
//arr：自动重装载初值
//psc：预分频系数
//溢出时间:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///开启定时器3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//预分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //时基初始化

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //开启TIM3更新中断
	TIM_Cmd(TIM3, ENABLE);					   //使能TIM3

	// NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //TIM3中断
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //主优先级
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //次优先级
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

//TIM3中断处理
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //中断
	{
		if(out_mode)  //角度
		{
			//pid1.SetPoint=out_msg;  //输入设定值
			SetAngle(out_msg);
		}
		else
		{
     pid2.SetPoint = pid2.SetPoint + 0.006 * out_msg;
		

	  pwmduty = PIDCalc(&pid2, Read_Encoder());
	  if (pwmduty > 0)
		  contrl_speed(pwmduty + 110, 1); //在2100的arr下，有80的死区
	  else
		  contrl_speed(pwmduty - 110, 1);
		}
		//TO DO
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除标志位
}

void ClearAll(void)
{
	    TIM2CH4_CAPTURE_STA &= 0x00; //清零
      angle = 0;                   //清零
      TIM4->CNT = 0x7FFF;          //
			pid1.SetPoint = 0;
			pid2.SetPoint = 0;
}
