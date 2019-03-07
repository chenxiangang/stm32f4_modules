#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "led.h"
#include "oled.h"
#include "pid.h"
#include "sys.h"
#include "timer.h" //实现控制功能的定时器
#include "usart.h"

FLAG_Typedef flag;

PID pid1;
PID pid2;
int a;
double pwmduty;
int angle0;
extern double Angle;

int main(void)
{
    pid1.SumError = 0;
    pid1.PrevError = 0;
    pid1.LastError = 0;
    pid1.LastError0 = 0;
    pid1.SetPoint = 0;

    pid2.SumError = 0;
    pid2.PrevError = 0;
    pid2.LastError = 0;
    pid2.LastError0 = 0;
    pid2.SetPoint = -3;

    pid1.Proportion = 0;
    pid1.Integral = 0.00;
    pid1.Derivative = 0;

    pid2.Proportion = 50;
    pid2.Integral = 0.04;
    pid2.Derivative = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
    delay_init(168); //初始化延时函数
    LED_Init(); //初始化LED端口
    uart_init(115200); //初始化串口1波特率为115200
    usart3_init(115200); //用来读取陀螺仪的数据
    TIM2_Int_Init(20, 7199); //初始化控制定时器
    Encoder_TIM4_Init();
    BTN7971_Init(1); //EN1组电机工作
    OLED_Init();
    SSI_init();

    while (1) {
        Angle_Cal();
        //强制转换成int类型才能在freecars上显示负数
        push(0, (int)roll * 10); //陀螺仪的值
        push(1, (int)Angle * 10);
        push(2, (int)Read_Encoder()); //电机转过的角度
        push(3, (int)pwmduty); //pwm输出
        push(4, (int)pid2.SetPoint * 10);
        uSendOnePage();
    }
}
