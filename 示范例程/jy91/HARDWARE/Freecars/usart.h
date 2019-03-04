
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//////////////////////////////////////////////////////////////////////////////////   ע�ⲻͬ���ڶ˿����߲�ͬ	

#define             macUSARTx2                                USART2
#define             macUSARTx_APBxClock_FUN2                   RCC_APB1PeriphClockCmd
#define             macUSARTx_CLK2                             RCC_APB1Periph_USART2
#define             macUSARTx_GPIO_AHBxClock_FUN2                    RCC_AHB1PeriphClockCmd
#define             macUSARTx_GPIO_CLK2                              RCC_AHB1Periph_GPIOD 
#define             macUSARTx_PORT2                                   GPIOD
#define             macUSARTx_TX2                                   GPIO_Pin_5
#define             macUSARTx_RX2                                   GPIO_Pin_6
#define             macGPIO_AF_USARTx2                               GPIO_AF_USART2	
#define             macGPIO_PinSourceTX2                           GPIO_PinSource5
#define             macGPIO_PinSourceRX2                           GPIO_PinSource6
#define             macUSART2_IRQn                           USART2_IRQn

#define             macUSARTx3                                USART3
#define             macUSARTx_APBxClock_FUN3                   RCC_APB1PeriphClockCmd
#define             macUSARTx_CLK3                             RCC_APB1Periph_USART3
#define             macUSARTx_GPIO_AHBxClock_FUN3                  RCC_AHB1PeriphClockCmd
#define             macUSARTx_GPIO_CLK3                              RCC_AHB1Periph_GPIOD 
#define             macUSARTx_PORT3                                   GPIOD
#define             macUSARTx_TX3                                   GPIO_Pin_8
#define             macUSARTx_RX3                                   GPIO_Pin_9
#define             macGPIO_AF_USARTx3                               GPIO_AF_USART3	
#define             macGPIO_PinSourceTX3                           GPIO_PinSource8
#define             macGPIO_PinSourceRX3                           GPIO_PinSource9
#define             macUSART3_IRQn                           USART3_IRQn



#define             macBluetooth_USARTx                                USART2


/*ע��FreeCarsԴ�ļ����õ���USART1δ�궨��*/

//#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
//	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound,u8 usart);
#endif




