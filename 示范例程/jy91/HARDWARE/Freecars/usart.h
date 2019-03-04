
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//////////////////////////////////////////////////////////////////////////////////   注意不同串口端口总线不同	

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


/*注意FreeCars源文件里用的是USART1未宏定义*/

//#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
//	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound,u8 usart);
#endif




