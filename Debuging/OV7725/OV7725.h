/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 20:12:01
 * @LastEditTime: 2019-04-05 22:06:32
 */
#ifndef _OV7725_H_
#define _OV7725_H_

#include "sys.h"
#include "stdbool.h"

typedef struct
{
    uint8_t addr; //寄存器地址
    uint8_t val;  //寄存器的值
} reg_7725;
typedef struct
{
 bool  ImgDealOk;
 bool  ImgOk;
 bool  InitStatu;
 s16   lines;
}CamCtrlClass;


#define CAMERA_W 20   //宽20
#define CAMERA_H 200  //高
#define CAMERA_SIZE         (CAMERA_W * CAMERA_H)
#define false 0
#define true  1

extern CamCtrlClass ImgControl;
extern u8 Image[CAMERA_H][CAMERA_W] ;

u8 OV7725_Init(void);
uint8_t OV7725_reg_Init(void);
#endif
