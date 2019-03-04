#include "include.h"
#include "FreeCars_uart.h"
#include "FreeCars_isr.h"
#include "usart.h"

/************һ���Ǹ���FreeCars��λ��Э���޸ĵģ�����ı�**************/
 uchar uSendBuf[UartDataNum*2 + LineDataNum]={0};
uchar FreeCarsDataNum=UartDataNum*2; //+ LineDataNum;
//uchar FreeCarsCount=0;
double UartData[9] = {0};
SerialPortType SerialPortRx;
/**************************/


// uchar UartGetOneChar(UART_MemMapPtr uartch)
uchar UartGetOneChar(void)
{
	 return USART_ReceiveData(macBluetooth_USARTx);		//��������	
}

/*
����һ���ֽ�
*/
// void USendOneChar (UART_MemMapPtr uartch, u8 ch)
void USendOneChar (u8 ch)
{
	USART_SendData(macBluetooth_USARTx, ch);  
	/* Loop until the end of transmission */  
	while(USART_GetFlagStatus(macBluetooth_USARTx, USART_FLAG_TXE) == RESET);    
}

/*
��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
*/
void uSendOnePage(void)
{
  uchar i,sum=0; 
  
  USendOneChar(251);
  USendOneChar(109);
  USendOneChar(37);//ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�֪���������
  sum+=(251);      //��ż����������У��
	sum+=(109);
  sum+=(37);
  for(i=0;i<FreeCarsDataNum/2;i++)
  {
    USendOneChar(uSendBuf[i*2]);
    USendOneChar(uSendBuf[i*2+1]);
    sum+=uSendBuf[i*2];//��ż����������У��
		sum+=uSendBuf[i*2+1];
  }
  USendOneChar(sum%256);
  
}

/*
��ĳ��ͨ���������
adr��ͨ��
date������-32768~32767
*/
void push(uchar adr,int32_t date)//int16  UInt16
{
  uSendBuf[adr*2]=date/256;
  uSendBuf[adr*2+1]=date%256;
}






