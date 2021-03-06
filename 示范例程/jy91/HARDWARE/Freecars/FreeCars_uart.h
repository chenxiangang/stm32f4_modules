#ifndef __FREECARS_UART_H__
#define __FREECARS_UART_H__

//1 头文件
// #include "common.h"
#include "stm32f4xx.h"
 
 
 #include "usart.h"

#define uchar			unsigned char
#define UInt16			uint16_t
//#define uint8			uint8_t

#define UartDataNum      17           //FreeCars上位机接收通道数，按照上位机设置改变!!!
#define LineDataNum      2            //FreeCars上位机赛道数据

//以下不要修改
#define UartRxBufferLen  100
#define UartRxDataLen    41           //FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen     7	      //FreeCars上位机接收命令数据长度，不要改

#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据
//尽管数据的长度只要求数组长度为26=22+3+1；但是为了与发送字符串取得统一
//取数据的长度与字符串的长度相等，这样在发送数据是会多发送了一些
//无效位，但是影响不大的
typedef struct 
{
  int Stack;
  uchar Data;
  uchar PreData;
  uchar Buffer[UartRxBufferLen];
  uchar Enable;
  uchar Check;
} SerialPortType;

extern uchar uSendBuf[UartDataNum*2 + LineDataNum];
extern SerialPortType SerialPortRx;
extern double UartData[9];

uchar UartGetOneChar(void);
void USendOneChar (u8 ch);  
void uSendOnePage(void);
void push(uchar adr,int32_t date);

void SendOV7620(void);
void SendOV7620_biImage(void);
#endif 
