#ifndef _RS485_H_
#define _RS485_H_
#include "sys.h"
//未经过测试

extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度
//这个是485的使能信号
#define RS485_RE PGout(8)
//这是最大接收
#define RS485_MAX_RECEIEVE  64

//这个自己写
void rs485_init(int bound);

//下面两个懒得写直接粘贴正点原子
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);

#endif
