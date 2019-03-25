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

#define deadband 70 //死区

double last_Left_Encoder_Angle = 0;
double last_Right_Encoder_Angle = 0;
double Left_Encoder_Angle = 0;
double Right_Encoder_Angle = 0;
int taskMode = 0; //任务模式
double pwmduty = 0;
double balance_point = 0; //平衡点

float PreSpeed;
float Encoder_Angle;

void changeTask(u8 taskid) //修改执行的任务
{
    taskMode = taskid;
}

void Tracking()
{
    YL_70_Read_All(YL70);
}

void RemoteControl()
{
}

void stop()
{
    pwmduty = 0;
    speedcontrol(0, LeftWheel, 0);
    speedcontrol(0, RightWheel, 0);          
}

void balance_UP(float Angle, float Gyro)
{
    double TempP;
    JYAngle_PID.PrevError = JYAngle_PID.SetPoint - Angle;

    if (JYAngle_PID.PrevError > 35 || JYAngle_PID.PrevError < -35) //大于25°就不调节了
    {
        JYAngle_PID.PrevError = 0;
        JYAngle_PID.SumError = 0;
        stop();
    }
    JYAngle_PID.SumError *= 0.2;
    JYAngle_PID.SumError += 0.8 * JYAngle_PID.PrevError;
    if (JYAngle_PID.SumError > 10000)
        JYAngle_PID.SumError = 10000;
    else if (JYAngle_PID.SumError < -10000)
        JYAngle_PID.SumError = -10000;

    if (JYAngle_PID.PrevError < 1&& JYAngle_PID.PrevError > -1) //小于2°也不调节了
        JYAngle_PID.PrevError = 0;
    //		else if(JYAngle_PID.PrevError > 6+balance_point || JYAngle_PID.PrevError <-6+balance_point)   //大于6°P增大
    //			  TempP = 1.4 * JYAngle_PID.Proportion;
    //		else if(JYAngle_PID.PrevError > 5+balance_point || JYAngle_PID.PrevError <-5+balance_point)   //大于5°P增大
    //			  TempP = 1.2 * JYAngle_PID.Proportion;
    //		else if(JYAngle_PID.PrevError > 4+balance_point || JYAngle_PID.PrevError <-4+balance_point)   //大于4°P增大
    TempP = JYAngle_PID.Proportion;
    //		else if(JYAngle_PID.PrevError > 3+balance_point || JYAngle_PID.PrevError <-3+balance_point)  //大于3°
    //			 TempP = 0.8 * JYAngle_PID.Proportion;
    //		else
    //				TempP = 1.5 * JYAngle_PID.Proportion;
    if (Gyro < 0.2 && Gyro > -0.2)
        Gyro = 0;
    JYAngle_PID.pwmduty = TempP * JYAngle_PID.PrevError + JYAngle_PID.SumError * JYAngle_PID.Integral - JYAngle_PID.Derivative * Gyro; //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数
}

void speed_UP()
{
    static int i = 0;
    static float Encoder;
    if (i == 2) {
        i = 0;
        Left_Encoder_Angle = Read_Encoder_L();
        Right_Encoder_Angle = Read_Encoder_R();
    } //要不要负号不知道
    i++;
    Speed_PID.PrevError = Left_Encoder_Angle + Right_Encoder_Angle - 0;
		if(Speed_PID.PrevError < 4 && Speed_PID.PrevError > -4)
		{
			Speed_PID.PrevError = 0;
		}
    Encoder *= 0.8;
    Encoder += Speed_PID.PrevError * 0.2;
    Speed_PID.SumError += Encoder;
//		Speed_PID.SumError *= 0.8;
    if (Speed_PID.SumError > 10000)
        Speed_PID.SumError = 10000; //限幅
    if (Speed_PID.SumError < -10000)
        Speed_PID.SumError = -10000;
    if (pitch > 35 || pitch < -35) {
        Speed_PID.PrevError = 0;
        Speed_PID.SumError = 0;
    }
//		if(JYAngle_PID.pwmduty < 80 && JYAngle_PID.pwmduty  > -80)
//		{
//				Speed_PID.PrevError = 0;
//        Speed_PID.SumError = 0;
//		}
    Speed_PID.pwmduty = Speed_PID.Proportion * Speed_PID.PrevError + Speed_PID.SumError * Speed_PID.Integral; //计算PID的值
}

//void pwm_pid()
//{
//	Speed_PID.PrevError = JYAngle_PID.pwmduty - Speed_PID.SetPoint;
//	Speed_PID.pwmduty = Speed_PID.Proportion * Speed_PID.PrevError + Speed_PID.SumError * Speed_PID.Integral; //计算PID的值
//}

void xianfu(double* pwmVal)
{
    if (*pwmVal > 1900)
        *pwmVal = 1900;
    else if (*pwmVal < -1900)
        *pwmVal = -1900;
}

void KeepBalance()
{
    balance_UP(pitch, gryo.y);
    speed_UP();
	//pwm_pid();
    pwmduty = JYAngle_PID.pwmduty + Speed_PID.pwmduty;
    xianfu(&pwmduty);
    speedcontrol(pwmduty, LeftWheel, deadband);
    speedcontrol(pwmduty, RightWheel, deadband);
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

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //使能指定的TIM5中断,允许更新中断

    TIM_ClearFlag(TIM5, TIM_IT_Update);

    TIM_Cmd(TIM5, ENABLE); //使能TIMx外设
}

//定时器3中断服务程序
void TIM5_IRQHandler(void) //TIM3中断
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        switch (taskMode) {
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
