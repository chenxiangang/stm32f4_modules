#include "control.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "encoder.h"
#include "math.h"
#include "pid.h"
#include "sys.h"
#include "tb6612.h"
#include "usart.h"

double Left_Encoder_Angle = 0;

void TIM5_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

    TIM_Cmd(TIM5, ENABLE); //使能TIMx外设
}
//定时器3中断服务程序
void TIM5_IRQHandler(void) //TIM3中断
{
    int i;
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        //        YL_70_Read_All(YL70);
        //        for (i = 0; i < 4; i++)
        //            printf("%d ", YL70[i]);
        //        printf("\r\n");
        Left_Encoder_Angle = Read_Encoder(1);
        printf("%f\r\n", Left_Encoder_Angle);

        TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
    }
}
