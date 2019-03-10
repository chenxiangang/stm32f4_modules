#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "mynvic.h" //中断优先级配置
#include "oled.h"
#include "pid.h"
#include "sys.h"
#include "timer.h" //实现控制功能的定时器
#include "usart.h"

FLAG_Typedef flag;

double MotoAngle;  //编码器外部变量

PID pid1;
PID PID_Moto;

extern double SSI_Angle; //16384编码器测得的角度

void Sys_Init(void) //初始化各模块和系统配置
{
  My_NVIC_Init();
  delay_init(168);   //初始化延时函数
  uart_init(115200); //初始化串口1波特率为115200
  //usart3_init(115200); //用来读取陀螺仪的数据
  //TIM2_Int_Init(100, 7199); //初始化控制定时器
  //初始化顺序交换中断里就不会读出负数
  Encoder_TIM4_Init();
  TIM2_Int_Init(100, 7199); //初始化控制定时器
  BTN7971_Init(1);          //EN1组电机工作
  OLED_Init();
  SSI_init();
}

void Pid_Init(void)
{
  pid1.SumError = 0;
  pid1.PrevError = 0;
  pid1.LastError = 0;
  pid1.LastError0 = 0;
  pid1.SetPoint = 0;

  PID_Moto.SumError = 0;
  PID_Moto.PrevError = 0;
  PID_Moto.LastError = 0;
  PID_Moto.LastError0 = 0;
  PID_Moto.SetPoint = 0;

  pid1.Proportion = 0;
  pid1.Integral = 0.00;
  pid1.Derivative = 0;

  PID_Moto.Proportion = 200;
  PID_Moto.Integral = 0.08;
  PID_Moto.Derivative = 10;
}

int main(void)
{
  //    double MotoAngle;
  Sys_Init();
  Pid_Init();
  while (1)
  {
    // 强制转换成int类型才能在freecars上显示负数
    //        push(1, (int)SSI_Angle * 10);
    //        push(2, (int)Read_Encoder() * 10); //pwm输出
    //        uSendOnePage();
    //        MotoAngle = Read_Encoder();  //这个读到的值收while函数运行时间的影响
    //        push(0, (int)SSI_Angle * 10);
    //        push(1, (int)MotoAngle * 10);
    //        push(2, (int)(SSI_Angle + MotoAngle) * 100);
    //        uSendOnePage();
    //       swing();
    printf("%f\r\n", MotoAngle);
    //printf("SSI_Angle:%f  MotoAngle:%f  Balance:%f\r\n",SSI_Angle,MotoAngle,SSI_Angle+MotoAngle);
  }
}
