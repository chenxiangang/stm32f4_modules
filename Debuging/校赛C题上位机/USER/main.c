/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-17 15:35:29
 * @LastEditTime: 2019-03-18 18:52:07
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mynvic.h"
#include "led.h"
#include "lcd.h"
#include "CAN.h"
#include "key.h"
#include "PWM.h"
/**
 * @description: �Ӵ����ж�ȡ���ò���
 * @param {char} t_mode ���ͷ�ʽ
 * @param {u8}   m_mode ת�� �Ƕ�
 * @param {u16}  msg    ������ֵ
 * @return: 0 û����   
 * 			1������
 * ����ͨ��
 */
int get_number(char *t_mode, int *m_mode, int *msg)
{
	if (USART_RX_STA & 0x8000) //�������
	{
		sscanf(USART_RX_BUF, "%c %d %d", t_mode, m_mode, msg); //��ȡ��ֵ  
		USART_RX_STA = 0x0000;								   //����
		return 1;
	}
	return 0;
}
/**
 * @description: ��ȡ���ڵõ�������תCCRֵ
 * @param {type}
 * motor_mode ģʽ
 * msg ��ת����Ϣ 
 * @return: CCR
 */
int trans_CCR(int motor_mode, int msg)
{
	int CCR;
	if (motor_mode) //ת�Ƕ�
	{
		CCR = msg * 5 + 2 + 500;
	}
	else //ת��
	{
		CCR = msg * 5 + 2 + 15000;
	}
	return CCR;
}

int main(void)
{
	char tans_mode;
	int motor_mode;
	int msg;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//����ϵͳ�ж����ȼ�����2
	delay_init(168);														//��ʼ����ʱ����
	uart_init(115200);														//��ʼ�����ڲ�����Ϊ115200
	My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN��ʼ��  ����ģʽ
	LED_Init();																//��ʼ��LED
	//LCD_Init();																//��ʼ��LCD FSMC�ӿ�
	KEY_Init();																//������ʼ��
	//POINT_COLOR = RED;														//������ɫ����ɫ
	//LCD_ShowString(100, 0, 16 * 13, 8, 16, "CURRENT_MODE:");				//LCD��ʾ
	TIM2_PWM_Init();
	while (1)
	{
		LED0 = !LED0;								   //��ʾ������������
		if (get_number(&tans_mode, &motor_mode, &msg)) //�յ����ڷ���
		{
			printf("return:%c %d %d\r\n", tans_mode, motor_mode, msg); //���ط���ֵ
			if ('C' == tans_mode || 'c' == tans_mode)				   //CANģʽ
			{
				CAN_SEND_CONTORL(motor_mode, msg); //����CAN��Ϣ
				TIM_SetCompare4(TIM2, 0);  //��0
			}
			else if ('P' == tans_mode || 'p' == tans_mode) //PWMģʽ
			{
				TIM_SetCompare4(TIM2, trans_CCR(motor_mode, msg)); //����PWM��Ϣ
			}
		}
		delay_ms(100);
	}
}
