/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-13 16:50:30
 * @LastEditTime: 2019-03-14 15:56:02
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "CAN.h"
#include "key.h"

#define buffer_len 2

int main(void)
{
	u8 send_buffer[buffer_len] = {1, 100};
	u8 receive_buffer[buffer_len]={0};
	u8 key;
	u8 temp_len;
	uart_init(115200);
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	KEY_Init();
	My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6,CAN_Mode_LoopBack); //CAN回环模式，波特率500kbps
	while (1)
	{
		key = KEY_Scan(0);
		if (WKUP_PRES == key)
		{
			CAN1_Send_Msg(send_buffer, buffer_len); //发送
			delay_ms(1000);
			temp_len = CAN1_Receive_Msg(receive_buffer); //接收
			if (temp_len)
			{
				printf("send:%d,%d\r\n",receive_buffer[0],receive_buffer[1]);
			}
		}
	}
}
