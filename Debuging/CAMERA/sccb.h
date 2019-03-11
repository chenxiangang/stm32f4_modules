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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//OV系列摄像头 SCCB 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//IO方向设置
//刚好是7不用改
//GPIO还是要改
#define SCCB_SDA_IN()  {GPIOA->MODER&=~(3<<(7*2));GPIOA->MODER|=0<<7*2;}	//PD7 输入
#define SCCB_SDA_OUT() {GPIOA->MODER&=~(3<<(7*2));GPIOA->MODER|=1<<7*2;} 	//PD7 输出


//IO操作函数	 
//按照省赛模板没改错的话是 SCL PA8     SDA PA7
#define SCCB_SCL    		PAout(8)	 	//SCL
#define SCCB_SDA    		PAout(7) 		//SDA	 

#define SCCB_READ_SDA    	PAin(7)  		//输入SDA    
#define SCCB_ID   			0X60  			//OV2640的ID  要改

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













