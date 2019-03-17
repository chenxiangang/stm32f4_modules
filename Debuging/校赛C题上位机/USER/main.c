/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-17 15:35:29
 * @LastEditTime: 2019-03-17 21:32:33
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mynvic.h"
#include "led.h"
#include "lcd.h"
#include "CAN.h"
#include "key.h"

/**
 * @description: 从串口中读取设置参数
 * @param {char} t_mode 发送方式
 * @param {u8}   m_mode 转速 角度
 * @param {u16}  msg    具体数值
 * @return: 0 没读到   
 * 			1读到了
 * 测试通过
 */
int get_number(char *t_mode, int *m_mode, int *msg)
{
	if (USART_RX_STA & 0x8000) //接收完成
	{
		sscanf(USART_RX_BUF, "%c %d %d", t_mode, m_mode, msg); //提取数值
		USART_RX_STA = 0x0000;								   //清零
		return 1;
	}
	return 0;
}

int main(void)
{
	char tans_mode;
	int motor_mode;
	int msg;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//设置系统中断优先级分组2
	delay_init(168);														//初始化延时函数
	uart_init(115200);														//初始化串口波特率为115200
	My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN初始化  正常模式
	LED_Init();																//初始化LED
	LCD_Init();																//初始化LCD FSMC接口
	KEY_Init();																//按键初始化
	POINT_COLOR = RED;														//画笔颜色：红色
	LCD_ShowString(100, 0, 16*13, 8, 16, "CURRENT_MODE:");					//LCD显示
	//sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
	while (1)
	{
		LED0 = !LED0;								   //表示程序正在运行
		if (get_number(&tans_mode, &motor_mode, &msg)) //收到串口发送
		{
			printf("return:%c %d %d\r\n", tans_mode, motor_mode, msg); //返回发送值
			if ('C' == tans_mode || 'c' == tans_mode)				   //CAN模式
			{
				CAN_SEND_CONTORL(motor_mode, msg); //发送CAN信息
			}
			else if ('P' == tans_mode || 'p' == tans_mode) //PWM模式
			{
				//TO DO
			}
		}
		delay_ms(100);
	}
}
