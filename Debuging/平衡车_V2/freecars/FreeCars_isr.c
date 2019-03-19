#include "FreeCars_isr.h"
#include "FreeCars_uart.h"

//extern float Balance_KP, Balance_KD, Position_KP, Position_KD; // PID系数
//extern u16 Moto1_1, Encoder;                                   //占空比0-1
//extern u16 Moto1_2, f, ff;                                     //占空比0-1
//extern int top_voltage, Position_Zero, xian_fu; //编码器的脉冲计数

void UartCmd(uint8 cmdnum, uint8 cmddata)       ///
{
  switch (cmddata) {
  case (6):
    break; // F6
  case (7):
    break; // F7
  case (8):
    break; // F8
  case (9):
    break; // F9
  case (10):
    break; // F10
  case (11):
    break; // F11
  case (12):
    break; // F12
  case (100):
    break; // PAUSE
  case (101):
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    break; // HOME
  case (102):
    break; // PG UP
  case (103):
    break; // PF DN
  case (104):
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    break; // END
  }
}
void UartDebug() {

//  top_voltage = UartData[0];
//  Position_Zero = UartData[2];
//  Balance_KP = UartData[4];
//  Balance_KD = UartData[5];
//  Position_KP = UartData[6];
//  Position_KD = UartData[7];
  //  bz_set(datarec);
}

void dealUartData(void) {
  int i, b, d1;
  UInt32 d;

  if (SerialPortRx.Stack < UartRxBufferLen) {
    SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;

    if (SerialPortRx.Stack >= UartRxDataLen // UartRxDataLen个数为一帧
        && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen] ==
               0xff //校验字头
        &&
        SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55 &&
        SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa &&
        SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10) {
      // double data 9个通道数据校验
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxDataLen; //起始位
      for (i = b; i < SerialPortRx.Stack - 1; i++) //除校验位外的位进行校验
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i]; //校验
      }

      if (SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1]) {
        //校验成功，进行数据解算
        for (i = 0; i < 9; i++) {
          d = SerialPortRx.Buffer[b + i * 4 + 4] * 0x1000000L +
              SerialPortRx.Buffer[b + i * 4 + 5] * 0x10000L +
              SerialPortRx.Buffer[b + i * 4 + 6] * 0x100L +
              SerialPortRx.Buffer[b + i * 4 + 7];
          if (d > 0x7FFFFFFF) {
            d1 = 0x7FFFFFFF - d;
          } else {
            d1 = d;
          }
          UartData[i] = d1;
          UartData[i] /= 65536.0;
        }
        UartDebug(); //转去处理，把受到的数据付给变量        特别注意
        push(2, (Int16)UartData[0]); //将数据发送回去观察
        push(3, (Int16)UartData[8]);
        uSendOnePage(); //发送一帧数据
      }
      SerialPortRx.Stack = 0;
    } else if (SerialPortRx.Stack >= UartRxCmdLen // UartRxDataLen个数为一帧
               &&
               SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen] == 0xff &&
               SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] ==
                   0x55 &&
               SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] ==
                   0xaa &&
               SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] ==
                   0x77) // cmd
    {
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxCmdLen;       //起始位
      for (i = b; i < SerialPortRx.Stack - 1; i++) //除校验位外的位进行校验
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i]; //校验
      }
      if (SerialPortRx.Check ==
          SerialPortRx.Buffer[SerialPortRx.Stack - 1]) { //校验成功
        UartCmd(UartCmdNum, UartCmdData); //处理接收到的命令，付给MCU命令变量
        //	push(0,UartCmdNum);	//将数据发送回去观察
        //	push(1,UartCmdData);
        //	uSendOnePage();//发送一帧数据
      }
      SerialPortRx.Stack = 0;
    }
  } else {
    SerialPortRx.Stack = 0;
  }
}


void USART1_IRQHandler(void) {
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);  //做什么用的？
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    SerialPortRx.Data = UartGetOneChar();
    dealUartData();
  }
}
