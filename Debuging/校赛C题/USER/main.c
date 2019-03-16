/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 14:29:02
 * @LastEditTime: 2019-03-16 14:34:42
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "mynvic.h"
#include "CAN.h"

#define top_psc (84 - 1)     //预分频初值
#define top_arr (0xFFFFFFFF) //重装载初值

//下位机程序
//目前案件转换
int main(void)
{
  int buffer_len = 0; //CAN读取的长度
  int t_mode = 1;     //通信方式 PWM 0  还是 CAN 1 初始CAN
  int out_mode;       //控制转速还是方向
  u16 out_msg;        //输出信息
  uart_init(115200);
  delay_init(168);
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN初始化
  TIM2_CH4_Cap_Init(top_psc, top_arr);                                    //输入捕获初始化  84MHz/84 1M开始计数
  //TO DO key初始化
  //TO DO 电机类的初始化
  while (1)
  {
    if (t_mode) //CAN模式
    {
      buffer_len = CAN_Receive_16(&out_mode, &out_msg); //读取信息
      if (buffer_len)                                   //读到了
      {
        //TO DO 控制信号处理
      }
    }
    else
    {

      //TO DO
    }
  }
}
