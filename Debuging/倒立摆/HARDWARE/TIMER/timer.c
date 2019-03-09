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

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                           //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM3, ENABLE); //使能TIMx外设
}
//定时器3中断服务程序
void TIM3_IRQHandler(void) //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
        LED1 = !LED1;
    }
}

void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                           //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM2, ENABLE); //使能TIMx外设
}

void TIM2_IRQHandler(void) //TIM2中断
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
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
        //				while(!(Angle < 10 && Angle > 350))  //等它稳定下来，重新起摆
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
            break; //起摆
        case 2:
            Revert();
            break; //倒立
        case 0:
            motor_action(0); //停止
            sPID.SumError = 0;
            break;
        }
    }
    uccr1 = nccr1;
    nccr1 = TIM4->CCR1; //有点问题
    speed = nccr1 - uccr1;
    if (speed < 0)
        speed = -speed;
    printf("%d\r\n", speed);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//TIM3 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //使能定时器3时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIO外设和AFIO复用功能模块时钟

    //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); //PB5

    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //复用
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM3 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //根据T指定的参数初始化外设TIM3 OC2

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器

    TIM_Cmd(TIM3, ENABLE); //使能TIM3
}
