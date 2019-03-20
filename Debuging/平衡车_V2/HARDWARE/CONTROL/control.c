#include "control.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "encoder.h"
#include "math.h"
#include "pid.h"
#include "sys.h"
#include "tb6612.h"
#include "usart.h"
#include "usmart.h"

double Left_Encoder_Angle = 0;
double Right_Encoder_Angle = 0;
int modeFlag = 0;
double pwmduty = 0;

float PreSpeed;
float Encoder_Angle;

void Tracking()
{
    YL_70_Read_All(YL70);
}

void RemoteControl()
{
}

//调节JYangle_PID,Speed_PID,Encoder_Angle_PID目标值均为0
void KeepBalance()
{
    //pitch,roll,yaw,speed(还要写一个函数)
    JYAngle_PID.SetPoint = 0;
    Speed_PID.SetPoint = 0;
    Encoder_Angle_PID.SetPoint = 0;
    PIDCalc(&JYAngle_PID, roll);
    PIDCalc(&Speed_PID, PreSpeed);
    PIDCalc(&Encoder_Angle_PID, Encoder_Angle);
    pwmduty = JYAngle_PID.pwmduty + Speed_PID.pwmduty + Encoder_Angle_PID.pwmduty;
    speedcontrol(pwmduty, LeftWheel, 110);
    speedcontrol(pwmduty, RightWheel, 110);
}

void stop()
{
    speedcontrol(0, LeftWheel, 0);
    speedcontrol(0, RightWheel, 0);
}

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
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
			printf("hello\r\n");
        switch (modeFlag) {
        case remote_control:
            RemoteControl();
            break;
        case tracking:
            Tracking();
            break;
        case keep_balance:
            KeepBalance();
            break;
        default:
            stop();
            break;
        }
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
    }
}
