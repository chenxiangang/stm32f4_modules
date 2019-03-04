#ifndef _RS485_H_
#define _RS485_H_
#include "sys.h"
//δ��������

extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���
//�����485��ʹ���ź�
#define RS485_RE PGout(8)
//����������
#define RS485_MAX_RECEIEVE  64

//����Լ�д
void rs485_init(int bound);

//������������дֱ��ճ������ԭ��
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);

#endif
