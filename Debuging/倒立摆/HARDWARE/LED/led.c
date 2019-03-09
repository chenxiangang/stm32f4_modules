/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 12:14:13
 * @LastEditTime: 2019-03-09 12:20:10
 */
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED0-->PB.5 �˿�����
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOB, GPIO_Pin_5);				   //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOE, &GPIO_InitStructure);	//������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOE, GPIO_Pin_5);		  //PE.5 �����

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��GPIO����ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11; //�������������ƶ˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			 //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;			 //����
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
