/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 14:29:02
 * @LastEditTime: 2019-03-18 20:58:29
 */
 
//0  转速
//1  角度
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "mynvic.h"
#include "CAN.h"
#include "timercap.h"
#include "led.h"
#include "btn7971.h"
#include "encoder.h"
#include "oled.h"
#include "pid.h"
#include "control.h"
#include "key.h"

#define top_psc (84 - 1)     //输入捕获预分频初值
#define top_arr (0xFFFFFFFF) //输入捕获重装载初值

int t_mode = 1; //通信方式 PWM 0  还是 CAN 1 初始CAN
u8 out_mode;    //控制转速还是方向
u16 out_msg;    //输出信息

PID pid1;
PID pid2;
extern double pwmduty;
//下位机程序
//目前案件转换
int main(void)
{
  //int buffer_len = 0; //CAN读取的长度

  pid1.SumError = 0; //pid参数初始化
  pid1.PrevError = 0;
  pid1.LastError = 0;
  pid1.LastError0 = 0;
  pid1.SetPoint = 0;

  pid1.Proportion = 3;
  pid1.Integral = 0.0;
  pid1.Derivative = 5;
	
	pid2.SumError = 0; //pid参数初始化
  pid2.PrevError = 0;
  pid2.LastError = 0;
  pid2.LastError0 = 0;
  pid2.SetPoint = 0;

  pid2.Proportion = 20;
  pid2.Integral = 0.0;
  pid2.Derivative = 0;
	
	
  uart_init(115200);
  delay_init(168);
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  BTN7971_Init(1);                                                          //电机驱动初始化
  Encoder_TIM4_Init();                                                      //编码器初始化
  OLED_Init();                                                              //oled初始化
  LED_Init();                                                               //LED初始化
  My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN初始化  CAN_Mode_Normal 正常模式
  TIM2_CH4_Cap_Init(top_psc, top_arr);                                      //输入捕获初始化  84MHz/84 1M开始计数
  KEY_Init();                                                               //按键初始化
	TIM3_Int_Init(1000-1,84-1);//1ms
  My_NVIC_Init();                                                           //中断配置  感觉里面优先级要注意一下
  while (1)
  {
    LED1 = !LED1;                                            //表示程序在运行
    OLED_ShowMPU(Read_Encoder(), pwmduty, pid1.Integral, 0); //显示数值
    if (KEY_Scan(0))                                         //不支持连续按 KEY0
    {
      t_mode = !t_mode;            //模式转换
      LED0 = !LED0;                //显示模式
      ClearAll();
			out_mode = 0;
      out_msg = 0;
    }
    // if (t_mode) //CAN模式
    // {
    //   buffer_len = CAN_Receive_16(&out_mode, &out_msg); //读取信息
    //   if (buffer_len)                                   //读到了
    //   {
    //     //TO DO 需要复位吗
    //     if (out_mode) //转角度
    //     {
    //       //TO DO 控制信号处理
    //     }
    //     else //转圈数
    //     {
    //     }
    //   }
    // }
    // else //PWM接收模式
    // {
    //   PWM_STA = PWM_Get_msg(&out_mode, &out_msg); //接收信号
    //   if (PWM_STA)                                //读到了
    //   {
    //     //TO DO 需要复位吗
    //     if (out_mode) //转角度
    //     {
    //       //TO DO 控制信号处理
    //     }
    //     else //转圈数
    //     {
    //     }
    //   }
    // }
    // //TO DO 保持原状
  }
}
