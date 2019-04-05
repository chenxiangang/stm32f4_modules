/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 12:16:47
 * @LastEditTime: 2019-04-05 13:40:02
 */
#ifndef _OV7620_H_
#define _OV7620_H_

#define	CAMERA_DMA_CH 	    DMA_CH0		    //定义摄像头的DMA采集通道
#define CAMERA_W            160            	//定义摄像头图像宽度
#define CAMERA_H            120             //定义摄像头图像高度
#define CAMERA_SIZE         (CAMERA_W * CAMERA_H)
#define false 0
#define true  1

typedef struct
{
	u8 Address;			       /*寄存器地址*/
	u8 Value;		           /*寄存器值*/
}Register_Info;

typedef struct
{
 bool  ImgDealOk;
 bool  ImgOk;
 bool  InitStatu;
 s16   lines;
}CamCtrlClass;

extern CamCtrlClass ImgControl;

extern u8 Image[CAMERA_H+5][CAMERA_W]= {0};  //图像

extern u8 Ov7620_reg_Init(void); //寄存器初始化
extern u8 OV7620_Init(void);   //初始化摄像头

#endif