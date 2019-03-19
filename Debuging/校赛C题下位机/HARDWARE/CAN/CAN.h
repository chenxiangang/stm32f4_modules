/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-11 20:17:38
 * @LastEditTime: 2019-03-18 20:25:49
 */
#ifndef _CAN_H_
#define _CAN_H_
#include "sys.h"

extern int t_mode;  //通信方式 PWM 0  还是 CAN 1 初始CAN
extern u8 out_mode; //控制转速还是方向
extern u16 out_msg; //输出信息

void My_CAN_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);
u8 CAN1_Send_Msg(u8 *msg, u8 len);
u8 CAN_SEND_CONTORL(u8 mode, u16 msg);
u8 CAN1_Receive_Msg(u8 *buf);
u8 CAN_Receive_16(u8 *mode, u16 *msg);
#endif
