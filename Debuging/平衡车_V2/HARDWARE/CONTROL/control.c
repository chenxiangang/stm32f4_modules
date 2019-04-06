#include "control.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "delay.h"
#include "encoder.h"
#include "math.h"
#include "pid.h"
#include "stm32f4xx_dac.h"
#include "sys.h"
#include "tb6612.h"
#include "usart.h"
#include "usmart.h"

#define deadband 0 //死区

int turn_flag = 3;
int turn_speed = 0;
int forward_flag = 3;
int forward_speed = 0;
u8 track_flag = 0;
int Left_Encoder_Angle = 0;
int Right_Encoder_Angle = 0;
double pwmduty1 = 0;
double pwmduty2 = 0;
double balance_point = 0; //平衡点

int last_YL3 = 0;
int last_YL0 = 0;
int cross_num = 0;
int corner_num = 0;
int corner_flag = 0; //判断拐直角的标志位,因为在直角转弯是可能会出现一段时间全空白的

int Forward_speed;
int Turn_speed;

void set_track_speed(int forward_speed, int turn_speed)
{
    Forward_speed = forward_speed;
    Turn_speed = turn_speed;
}

void Tracking()
{
    YL_70_Read_All(YL70);
    //---------------------------------------------------------------
    //0是没检测到黑线，灯亮
//    if (last_YL0 == 0 && YL70[0] == 1) { //进十字
//        cross_num++;
//    }
//    if (last_YL0 == 1 && YL70[0] == 0) { //出十字
//        cross_num++;
//    }

    if (last_YL3 == 0 && YL70[3] == 1) //由白到黑跳变,直角或者十字
    {
        corner_flag = 1;
        //corner_num++;
    }

    if (cross_num == 1) { //第一次十字,继续走
        forward(0, 550);
    } else if (cross_num == 4) { //第二次出十字，停
        forward_flag = 2;
        Speed_PID.SumError = 1500;
        turn_speed = 0;
        track_flag = 0;
    } else if (corner_flag) //直角
    {
        if (corner_num == 1) //起点,直走
        {
            forward(0, 550);
            corner_flag = 0;
        } else {
            forward(1, 0);
            turn(LEFT, 2500);
        }
        if (YL70[2]) //转弯结束条件
        {
            corner_flag = 0;
            turn(RIGHT, 0);
        }
    } else //非直角
    {
        if (!(YL70[0] | YL70[1] | YL70[2] | YL70[3])) //全部没检测到，直角转弯时也会出现，居中或者离开区域
        {
            forward(0, 550);
            turn(0, 0);
        } else if (YL70[2] && !(YL70[0] | YL70[1] | YL70[3])) //中间偏左检测到黑线 2
        {
            turn(LEFT, 1000); //右转
        } else if (YL70[1] && !(YL70[0] | YL70[2] | YL70[3])) //中间偏右检测到黑线 1
        {
            turn(RIGHT, 1000); //左转
        } else if (YL70[1] && YL70[2]) {
            forward(0, 550);
            turn(0, 0);
        }
    }
    last_YL0 = YL70[0];
    last_YL3 = YL70[3];
    //----------------------------能跑完，但是比较蛇皮--------------------------------------
    //    if (last_YL0 == 0 && YL70[0] == 1) { //进十字
    //        cross_num++;
    //    }
    //    if (last_YL0 == 1 && YL70[0] == 0) { //出十字
    //        cross_num++;
    //    }

    //    if (cross_num == 1) { //第一次十字,继续走
    //        forward(0, 900);
    //    } else if (cross_num == 4) { //第二次出十字，停
    //        forward_flag = 2;
    //        Speed_PID.SumError = 1500;
    //        turn_speed = 0;
    //        track_flag = 0;
    //    } else {
    //        if (YL70[2] == 1) { //中间黑线，直走
    //            forward(0, 550);
    //            turn(LEFT, 0);
    //        } else if (YL70[1] == 1 && !YL70[3]) { //偏右，右转
    //					forward(0,0);
    //            turn(RIGHT, 1200);
    //        } else if (YL70[3] == 1 && !YL70[1]) { //偏左，左转
    //            turn(LEFT, 1200);
    //					forward(0,0);
    //        } else if (YL70[1] && YL70[3]) { //直角，左转
    //            turn(LEFT, 1200);
    //					forward(0,0);
    //        }
    //    }
    //last_YL0 = YL70[0];
    //---------------------------------------------------------------
}

void stop()
{
    pwmduty1 = 0;
    pwmduty2 = 0;
    speedcontrol(0, LeftWheel, 0);
    speedcontrol(0, RightWheel, 0);
    forward_speed = 0;
    turn_speed = 0;
}

void balance_UP(float Angle, float Gyro)
{
    double TempP;
    JYAngle_PID.PrevError = JYAngle_PID.SetPoint - Angle;

    if (JYAngle_PID.PrevError > 30 || JYAngle_PID.PrevError < -30) //大于25°就不调节了
    {
        JYAngle_PID.PrevError = 0;
        JYAngle_PID.SumError = 0;
        stop();
    }
    TempP = JYAngle_PID.Proportion;
    if (JYAngle_PID.PrevError < 0.1 && JYAngle_PID.PrevError > -0.1) //小于2°也不调节了
        JYAngle_PID.PrevError = 0;
    else if (JYAngle_PID.PrevError > 18 + balance_point || JYAngle_PID.PrevError < -18 + balance_point)
        TempP = 1.9 * JYAngle_PID.Proportion;
    else
        TempP = JYAngle_PID.Proportion;
    JYAngle_PID.pwmduty = TempP * JYAngle_PID.PrevError - JYAngle_PID.Derivative * Gyro;
    //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数
}

void speed_UP()
{
    static double Encoder = 0;
    Speed_PID.PrevError = Left_Encoder_Angle + Right_Encoder_Angle - Speed_PID.SetPoint;
    Encoder *= 0.8;
    Encoder += Speed_PID.PrevError * 0.2;
    Speed_PID.SumError += Encoder;
    if (Speed_PID.SumError > 60000)
        Speed_PID.SumError = 60000; //限幅
    if (Speed_PID.SumError < -60000)
        Speed_PID.SumError = -60000;
    if (pitch > 30 || pitch < -30) {
        Speed_PID.PrevError = 0;
        Speed_PID.SumError = 0;
        Encoder = 0;
    }
    Speed_PID.pwmduty = Speed_PID.Proportion * Speed_PID.PrevError + Speed_PID.SumError * Speed_PID.Integral - Speed_PID.Derivative * (Speed_PID.PrevError - Speed_PID.LastError); //计算PID的值
    Speed_PID.LastError = Speed_PID.PrevError;
}

void xianfu(double* pwmVal)
{
    if (*pwmVal > 8400)
        *pwmVal = 8400;
    else if (*pwmVal < -8400)
        *pwmVal = -8400;
}

//转向遥控
void turn_ctl()
{
    if (turn_flag == LEFT || turn_flag == RIGHT) {
        if (turn_flag == LEFT) {
            pwmduty1 -= turn_speed;
            pwmduty2 += turn_speed;
        } else {
            pwmduty1 += turn_speed;
            pwmduty2 -= turn_speed;
        }
    }
}

//前进遥控
void forward_ctl()
{
    if (forward_flag == 0)
        Speed_PID.SumError = forward_speed;
    if (forward_flag == 1)
        Speed_PID.SumError = -forward_speed;
}

void KeepBalance()
{
    balance_UP(pitch, gryo.y); //角度环
    speed_UP(); //速度环
    pwmduty1 = JYAngle_PID.pwmduty + Speed_PID.pwmduty;
    pwmduty2 = JYAngle_PID.pwmduty + Speed_PID.pwmduty;
    if (track_flag) {
        Tracking();
    } else {
        corner_flag = 0;
        corner_num = 0;
        last_YL3 = 0;
        last_YL0 = 0;
        cross_num = 0;
    }
    //---------------运动控制，可以遥控或者循迹来控制
    turn_ctl(); //转向
    forward_ctl();
    //----------------
    xianfu(&pwmduty1);
    xianfu(&pwmduty2);
    speedcontrol(pwmduty1, LeftWheel, deadband);
    speedcontrol(pwmduty2, RightWheel, deadband);
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

//定时执行任务
void TIM5_IRQHandler(void) //TIM3中断
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        KeepBalance();
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
    }
}

//定时读编码器
void TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = 50; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = 8400; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //使能指定的TIM5中断,允许更新中断

    TIM_ClearFlag(TIM7, TIM_IT_Update);

    TIM_Cmd(TIM7, ENABLE); //使能TIMx外设
}
//定时器7中断服务程序
void TIM7_IRQHandler(void) //TIM3中断
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        Left_Encoder_Angle = Read_Encoder_L();
        Right_Encoder_Angle = Read_Encoder_R();

        TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //清除TIMx的中断待处理位:TIM 中断源
    }
}
