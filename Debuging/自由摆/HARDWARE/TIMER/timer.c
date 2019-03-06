/*------------------------
 用TIM2中断来执行控制操作
 需要初始化TIM2_Int_Init(arr,psc);
 把控制内容放到中断函数里面去
------------------------*/
#include "timer.h"

//-------------------添加需要的头文件---------------
#include "control.h"
#include "led.h"
#include "usart.h"
#include "SSI4096.h"
#include "delay.h"
#include "JY901_uart.h"
#include "btn7971.h"
#include "pid.h"
//-------------------------------------------------

//------------------引用的外部变量-----------------
extern double Angle;
extern PID pid1;
extern PID pid2;
extern int speed;
extern int last_speed;
extern double pwmduty;
extern u16 realpwmduty;
extern double dutychange2;
extern double    rout;
extern u16 uccr1;
extern u16 nccr1;
extern PID sPID;
//--------------------------------------------------


void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM2, ENABLE); //使能TIMx外设
}

void TIM2_IRQHandler(void) //TIM2中断
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
			Angle_Cal();
			if(pid2.SetPoint < 20 || pid2.SetPoint > -20)
				pid2.SetPoint = -Angle * 0.5;
			if(pid2.SetPoint >20  &&pid2.SetPoint <40  || pid2.SetPoint <-20 && pid2.SetPoint > -40)
				pid2.SetPoint = -Angle * 0.6;
			if(Angle > 40 || Angle < -40)
				pid2.SetPoint = -Angle * 0.7;
		
			pwmduty = PIDCalc(&pid2,-roll);
		
		  if(pwmduty > 0)
				contrl_speed(pwmduty + 80,1);   //100为死区
			else if (pwmduty < 0)
				contrl_speed(pwmduty - 80,1);   //100为死区
			
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

