/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-11 20:17:38
 * @LastEditTime: 2019-03-13 16:20:29
 */
#ifndef _CAN_H_
#define _CAN_H_
#include "sys.h"


void My_CAN_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);
u8 CAN1_Send_Msg(u8 *msg, u8 len);
u8 CAN1_Receive_Msg(u8 *buf);
u8 CAN_Receive_16(u8 *mode, u16 *msg);
#endif
