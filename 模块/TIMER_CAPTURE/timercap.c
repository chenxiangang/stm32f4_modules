/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-13 19:56:57
 * @LastEditTime: 2019-03-13 23:50:03
 */
#include "timercap.h"

void TIM5_CH1_Cap_Init(u16 psc, u16 arr)
{
    GPIO_InitTypeDef GPIO_InitStructure;           //GPIO结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //时基结构体
    TIM_ICInitTypeDef TIM_ICInitStructure;         //时钟输入结构体
#ifndef _MYNVIC_H_
    NVIC_InitTypeDef NVIC_InitStructure; //中断结构体
#endif

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //开启GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //开启TIM5

    //结构体初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //PA0
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化GPIO

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //复用开启

    //时基初始化
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //一分频，不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseStructure.TIM_Period = arr;                     //重装载初值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //预分频系数
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);             //初始化时基

    //捕获参数初始化
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                //通道1
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                        //不滤波
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿捕获，可按需求修改
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI
    TIM_ICInit(TIM5, &TIM_ICInitStructure);                         //捕获参数初始化

    TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE); //允许中断更新，

#ifndef _MYNVIC_H_
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#endif

    TIM_Cmd(TIM5, ENABLE); //使能
}
//[7] 1成功捕获高电平一次
//[6] 1捕获到上升沿
//[5:0] 捕获到上升沿后溢出个数
u8 TIM5CH1_CAPTURE_STA = 0; //输入捕获状态
u32 TIM5CH1_CAPTURE_VAL;    //输入捕获值 TIM2/TIM5 32位
//中断处理函数
void TIM5_IRQHandler(void)
{
    if (TIM5CH1_CAPTURE_STA & 0x80 == 0) //未成功捕获
    {
        if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //溢出
        {
            if (TIM5CH1_CAPTURE_STA & 0X40) //捕获到高电平
            {
                if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F) //太长的高电平
                {
                    TIM5CH1_CAPTURE_STA |= 0X80; //标记成功捕获
                    TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
                }
                else
                    TIM5CH1_CAPTURE_STA++;
            }
        }
        if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) //捕获事件发生
        {
            if (TIM5CH1_CAPTURE_STA & 0X40) //捕获到下降沿
            {
                TIM5CH1_CAPTURE_STA |= 0X80;                        //标记捕获到一次完整高电平
                TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);        //当前捕获值
                TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising); //CC1P=0 设置上升沿捕获
            }
            else //未开始，第一次捕获上升沿
            {
                TIM5CH1_CAPTURE_STA = 0; //清空
                TIM5CH1_CAPTURE_VAL = 0;
                TIM5CH1_CAPTURE_STA |= 0X40; //标记捕获到上升沿
                TIM_Cmd(TIM5, DISABLE);      //关闭定时器5
                TIM_SetCounter(TIM5, 0);
                TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling); //CC1P=1 设置为下降沿捕获
                TIM_Cmd(TIM5, ENABLE);                               //使能定时器5
            }
        }
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update); //中断清除
}