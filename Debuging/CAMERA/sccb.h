/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 18:46:03
 * @LastEditTime: 2019-03-11 18:00:08
 */
#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//OVϵ������ͷ SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//IO��������
//�պ���7���ø�
//GPIO����Ҫ��
#define SCCB_SDA_IN()  {GPIOA->MODER&=~(3<<(7*2));GPIOA->MODER|=0<<7*2;}	//PD7 ����
#define SCCB_SDA_OUT() {GPIOA->MODER&=~(3<<(7*2));GPIOA->MODER|=1<<7*2;} 	//PD7 ���


//IO��������	 
//����ʡ��ģ��û�Ĵ�Ļ��� SCL PA8     SDA PA7
#define SCCB_SCL    		PAout(8)	 	//SCL
#define SCCB_SDA    		PAout(7) 		//SDA	 

#define SCCB_READ_SDA    	PAin(7)  		//����SDA    
#define SCCB_ID   			0X60  			//OV2640��ID  Ҫ��

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













