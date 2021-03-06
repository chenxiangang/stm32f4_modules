#include "include.h"
#include "FreeCars_uart.h"
#include "FreeCars_isr.h"
#include "usart.h"

/************一下是根据FreeCars上位机协议修改的，无需改变**************/
 uchar uSendBuf[UartDataNum*2 + LineDataNum]={0};
uchar FreeCarsDataNum=UartDataNum*2; //+ LineDataNum;
//uchar FreeCarsCount=0;
double UartData[9] = {0};
SerialPortType SerialPortRx;
/**************************/


// uchar UartGetOneChar(UART_MemMapPtr uartch)
uchar UartGetOneChar(void)
{
	 return USART_ReceiveData(macBluetooth_USARTx);		//接收数据	
}

/*
发送一个字节
*/
// void USendOneChar (UART_MemMapPtr uartch, u8 ch)
void USendOneChar (u8 ch)
{
	USART_SendData(macBluetooth_USARTx, ch);  
	/* Loop until the end of transmission */  
	while(USART_GetFlagStatus(macBluetooth_USARTx, USART_FLAG_TXE) == RESET);    
}

/*
轮询法发送一帧数据
消耗时间与数据长度有关
不可以放在中断里面中期性调用
*/
void uSendOnePage(void)
{
  uchar i,sum=0; 
  
  USendOneChar(251);
  USendOneChar(109);
  USendOneChar(37);//使用轮询的方式发送数据，当数据未发送，程序停在此处知道发送完成
  sum+=(251);      //第偶数个数加入校验
	sum+=(109);
  sum+=(37);
  for(i=0;i<FreeCarsDataNum/2;i++)
  {
    USendOneChar(uSendBuf[i*2]);
    USendOneChar(uSendBuf[i*2+1]);
    sum+=uSendBuf[i*2];//第偶数个数加入校验
		sum+=uSendBuf[i*2+1];
  }
  USendOneChar(sum%256);
  
}

/*
向某个通道填充数据
adr：通道
date：数据-32768~32767
*/
void push(uchar adr,int32_t date)//int16  UInt16
{
  uSendBuf[adr*2]=date/256;
  uSendBuf[adr*2+1]=date%256;
}






