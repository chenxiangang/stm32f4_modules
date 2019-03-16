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
#include "CAN.h"

int main(void)
{
  uart_init(115200);
  delay_init(168);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN初始化
  while (1)
  {
  }
}
