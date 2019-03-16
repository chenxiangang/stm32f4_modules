/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 20:10:50
 * @LastEditTime: 2019-03-16 20:19:14
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "mynvic.h"

int main(void)
{
  uart_init(115200);
  delay_init(168);
  //TO DO 模块初始化
  //TO DO 直立
  //TO DO 循迹
  while (1)
  {
    //TO DO 显示参数
    //TO DO 无线控制
  }
}
