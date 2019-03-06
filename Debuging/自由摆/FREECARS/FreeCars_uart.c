#include "FreeCars_uart.h"

/************һ���Ǹ���FreeCars��λ��Э���޸ĵģ�����ı�**************/
uchar uSendBuf[UartDataNum * 2 + LineDataNum] = {0};
uchar FreeCarsDataNum = UartDataNum * 2; //+ LineDataNum;
// uchar FreeCarsCount=0;
double UartData[9] = {0};
SerialPortType SerialPortRx;
/**************************/

// uchar UartGetOneChar(UART_MemMapPtr uartch)
uchar UartGetOneChar(void) {
  return USART_ReceiveData(USART1); //��������
}

/*
����һ���ֽ�
*/
// void USendOneChar (UART_MemMapPtr uartch, uint8 ch)
void USendOneChar(uint8 ch) {
  USART_SendData(USART1, ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;
}

/*
��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
*/
void uSendOnePage(void) {
  uchar i, sum = 0;

  USendOneChar(251);
  USendOneChar(109);
  USendOneChar(
      37); //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�֪���������
  sum += (251); //��ż����������У��
  sum += (109);
  sum += (37);
  for (i = 0; i < FreeCarsDataNum / 2; i++) {
    USendOneChar(uSendBuf[i * 2]);
    USendOneChar(uSendBuf[i * 2 + 1]);
    sum += uSendBuf[i * 2]; //��ż����������У��
    sum += uSendBuf[i * 2 + 1];
  }
  USendOneChar(sum % 256);
}

/*
��ĳ��ͨ���������
adr��ͨ��
date������-32768~32767
*/
void push(uchar adr, UInt16 date) {
  uSendBuf[adr * 2] = date / 256;
  uSendBuf[adr * 2 + 1] = date % 256;
}

