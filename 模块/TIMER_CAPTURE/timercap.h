/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-13 19:57:06
 * @LastEditTime: 2019-03-13 23:51:13
 */
#ifndef _TIMERCAP_H
#define _TIMERCAP_H
#include "sys.h"
//输入捕获
//[7] 1成功捕获高电平一次  0未捕获
//[6] 1捕获到上升沿 0未捕获
//[5:0] 捕获到上升沿后溢出个数
extern  u8 TIM5CH1_CAPTURE_STA = 0; //输入捕获状态
extern  u32 TIM5CH1_CAPTURE_VAL;    //输入捕获值 TIM2/TIM5 32位


void TIM5_CH1_Cap_Init(u16 psc, u16 arr);  //初始化函数

#endif