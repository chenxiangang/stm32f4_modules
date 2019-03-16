/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-13 19:56:57
 * @LastEditTime: 2019-03-13 23:50:03
 */
#include "timercap.h"

void TIM2_CH4_Cap_Init(u16 psc, u32 arr)
{
    GPIO_InitTypeDef GPIO_InitStructure;           //GPIO结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //时基结构体
    TIM_ICInitTypeDef TIM_ICInitStructure;         //时钟输入结构体
#ifndef _MYNVIC_H_
    NVIC_InitTypeDef NVIC_InitStructure; //中断结构体
#endif

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //开启GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //开启TIM5

    //结构体初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;         //PB11
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化GPIO

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2); //复用开启

    //时基初始化
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //一分频，不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseStructure.TIM_Period = arr;                     //重装载初值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //预分频系数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //初始化时基

    //捕获参数初始化
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;                //通道4
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                        //不滤波
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿捕获，可按需求修改
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI
    TIM_ICInit(TIM2, &TIM_ICInitStructure);                         //捕获参数初始化

    TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE); //允许中断更新，

#ifndef _MYNVIC_H_
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#endif

    TIM_Cmd(TIM2, ENABLE); //使能
}
//[7] 1成功捕获高电平一次
//[6] 1捕获到上升沿
//[5:0] 捕获到上升沿后溢出个数
u8 TIM2CH4_CAPTURE_STA = 0; //输入捕获状态
u32 TIM2CH4_CAPTURE_VAL;    //输入捕获值 TIM2/TIM5 32位
//中断处理函数
void TIM2_IRQHandler(void)
{
    if (TIM2CH4_CAPTURE_STA & 0x80 == 0) //未成功捕获
    {
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //溢出
        {
            if (TIM2CH4_CAPTURE_STA & 0X40) //捕获到高电平
            {
                if ((TIM2CH4_CAPTURE_STA & 0X3F) == 0X3F) //太长的高电平
                {
                    TIM2CH4_CAPTURE_STA |= 0X80; //标记成功捕获
                    TIM2CH4_CAPTURE_VAL = 0XFFFFFFFF;
                }
                else
                    TIM2CH4_CAPTURE_STA++;
            }
        }
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) //捕获事件发生
        {
            if (TIM2CH4_CAPTURE_STA & 0X40) //捕获到下降沿
            {
                TIM2CH4_CAPTURE_STA |= 0X80;                        //标记捕获到一次完整高电平
                TIM2CH4_CAPTURE_VAL = TIM_GetCapture1(TIM2);        //当前捕获值
                TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising); //CC1P=0 设置上升沿捕获 通道4
            }
            else //未开始，第一次捕获上升沿
            {
                TIM2CH4_CAPTURE_STA = 0; //清空
                TIM2CH4_CAPTURE_VAL = 0;
                TIM2CH4_CAPTURE_STA |= 0X40; //标记捕获到上升沿
                TIM_Cmd(TIM2, DISABLE);      //关闭定时器5
                TIM_SetCounter(TIM2, 0);
                TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling); //CC1P=1 设置为下降沿捕获 通道4
                TIM_Cmd(TIM2, ENABLE);                               //使能定时器2
            }
        }
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update); //中断清除
}

//计高电平时间
//返回高电平的时间
u32 Time_Counter()
{
    u32 counter = 0;
    if (TIM2CH4_CAPTURE_STA & 0X80) //捕捉到了高电平
    {
        counter = TIM2CH4_CAPTURE_STA & 0X3F;
        counter *= 0XFFFFFFFF;          //溢出时间总合 top_arr的值
        counter += TIM2CH4_CAPTURE_VAL; //等到总的高电平时间
        TIM2CH4_CAPTURE_STA = 0;        //开启下次捕获
    }
    return counter;
}

//类似CAN通信一样读数据
u8 PWM_Get_msg(u8 *mode,u16 *msg)
{
    //TO DO
}