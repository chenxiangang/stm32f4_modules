#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "led.h"
#include "mynvic.h"
#include "oled.h"
#include "pid.h"
#include "sys.h"
#include "tb6612.h"
#include "usart.h"
#include "usmart.h"

/*
光电对管： PD0 PD1 PD2 PD3 
ENCODER1：   PD12 PD13  (TIM4)
ENCODER2： 待定
TB6612：PWM：PA6 PA7(目前与LED冲突)   (TIM3)          AIN1 AIN2 BIN1 BIN2   PA0 PA1 PA2 PA3    STBY接电源3.3V
JY901:     USART3  TX PD8    RX PD9     
OLED:   CS  PD3 
        RST PD4
				DC  PD5
				D0  PD6
				D1  PD7

*/

FLAG_Typedef flag;
int sendflag = 0; //发送信息标志
//-----------------usmart调试用------------
void JY_changePID(int p, int i, int d)
{
    JYAngle_PID.Proportion = p;
    JYAngle_PID.Integral = 1.0 * i / 1000;
    JYAngle_PID.Derivative = 1.0 * d / 100;
}
void Speed_changePID(int p, int i, int d)
{
    Speed_PID.Proportion = 1.0 * p / 100;
    Speed_PID.Integral = 1.0 * i / 100;
    Speed_PID.Derivative = 1.0 * d / 100;
}
void Turn_changePID(int p, int i, int d)
{
    Turn_PID.Proportion = 1.0 * p / 100;
    Turn_PID.Integral = 1.0 * i / 100;
    Turn_PID.Derivative = 1.0 * d / 100;
}
void send_info(void)
{
    sendflag = !sendflag;
}

void change_balancePoint(u8 flag, u8 val10)
{
    if (flag == 0)
        balance_point = -1.0 * val10 / 10;
    else
        balance_point = 1.0 * val10 / 10;
    JYAngle_PID.SetPoint = balance_point;
}
void forward(u16 flag, u16 dist)
{
    forward_flag = flag;
    forward_speed = dist;
}
void turn(u16 flag, u16 dist)
{
    turn_flag = flag;
    turn_speed = dist;
}
//停止转向和前进后退
void stop_ctl(void)
{

    forward_flag = 2;
    Speed_PID.SumError = 0;
    turn_speed = 0;
}

void track_ctl()
{
    track_flag = !track_flag;
    if (track_flag) {
        printf("ON\r\n");
    } else
        printf("OFF\r\n");
}
//------------------------------------
void PID_Init()
{
    //角度环参数初始化
    JYAngle_PID.SetPoint = balance_point;
    JYAngle_PID.LastError0 = 0;
    JYAngle_PID.LastError = 0;
    JYAngle_PID.PrevError = 0;
    JYAngle_PID.SumError = 0;
    JYAngle_PID.pwmduty = 0;
    //速度环参数初始化
    Speed_PID.SetPoint = 0;
    Speed_PID.LastError0 = 0;
    Speed_PID.LastError = 0;
    Speed_PID.PrevError = 0;
    Speed_PID.SumError = 0;
    Speed_PID.pwmduty = 0;

    //角度环PID
    JYAngle_PID.Proportion = 0;
    JYAngle_PID.Integral = 0;
    JYAngle_PID.Derivative = 0;

    //速度环PID
    Speed_PID.Proportion = 0;
    Speed_PID.Integral = 0;
    Speed_PID.Derivative = 0;
}

int main(void)
{
    delay_init(168); //初始化延时函数
    My_NVIC_Init(); //配置中断优先级
    LED_Init(); //初始化LED端口
    PID_Init(); //PID初始化
    usmart_dev.init(84); //初始化USMART
    YL_70_Init(); //初始化光电对管
    Encoder_TIM2_Init(); //初始化电机编码器B
    Encoder_TIM4_Init(); //初始化电机编码器A
    TIM5_Init(5000 - 1, 84 - 1); //读取传感器数据，进行pid控制
    uart_init(115200); //初始化串口1，用于发送数据到上位机
    usart3_init(115200); //用来读取陀螺仪的数据
    TB6612_Init(); //电机驱动初始化
    OLED_Init(); //OLED初始化
    TIM7_Init();
    JY_changePID(200, 0, 7000); //  这是乘0.6前的稳JY_changePID(500,0,11500)
    Speed_changePID(32000, 160, 0);
    Turn_changePID(0, 0, 0);
	track_ctl();
    while (1) {
        //        YL_70_Read_All(YL70);
        //        for (i = 0; i < 4; i++) {
        //            printf("%d	", YL70[i]);
        //        }
			printf("YL0:%d\r\n",YL70[0]);
			printf("crossnum:%d\r\n",cross_num);
			printf("cornernum:%d\r\n",corner_num);
			delay_ms(300);
        //        OLED_ShowMPU(Left_Encoder_Angle,Right_Encoder_Angle,pitch,Speed_PID.SumError);
        //        //        push(0, (int)pitch);
        //        //        push(1, (int)JYAngle_PID.pwmduty);
        //        //        uSendOnePage();
        //			//printf("Encoder:%f  %f\r\n", Read_Encoder_L(), Read_Encoder_R());
								if (sendflag) {
										printf("pitch:%f	gyro:%f	pwmduty:%f\r\nspeed_PID.pwmduty:%f\r\n", pitch, gryo.y, pwmduty1, Speed_PID.pwmduty);
									 // printf("%lf	%f\r\n", Left_Encoder_Angle, Right_Encoder_Angle);
										printf("Angle PrevError:%lf\r\n", JYAngle_PID.PrevError);
									printf("Pitch:%lf\r\n", pitch);
									printf("Encoder_SumError:%f\r\n",Speed_PID.SumError);
										printf("Encoder:%d  %d\r\n\r\n", Left_Encoder_Angle, Right_Encoder_Angle);
				delay_ms(300);
             }
    }
}
