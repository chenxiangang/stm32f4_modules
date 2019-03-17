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
extern u8 TIM2CH4_CAPTURE_STA ; //输入捕获状态
extern u32 TIM2CH4_CAPTURE_VAL;    //输入捕获值 TIM2/TIM5 32位

void TIM2_CH4_Cap_Init(u16 psc, u32 arr); //初始化函数
u32 Time_Counter(void);                       //读取捕获值
u8 PWM_Get_msg(u8 *mode, u16 *msg);       //PWM读数据
#endif
