#include "FreeCars_isr.h"
#include "FreeCars_uart.h"
#include "DCMI.h"
#include "OV7620.h"

/************一下是根据FreeCars上位机协议修改的，无需改变**************/
uchar uSendBuf[UartDataNum*2 + LineDataNum]={0};
uchar FreeCarsDataNum=UartDataNum*2; //+ LineDataNum;
//uchar FreeCarsCount=0;
double UartData[9] = {0};
SerialPortType SerialPortRx;
/**************************/


void uart2_init(u32 bound)
{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5复用为USART2 TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6复用为USART2 RX
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOD5与GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD5，PD6

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(USART2, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

// uchar UartGetOneChar(UART_MemMapPtr uartch)
uchar UartGetOneChar(void)
{
	return USART_ReceiveData(USART2);		//接收数据	
}

/*
发送一个字节
*/
// void USendOneChar (UART_MemMapPtr uartch, uint8 ch)
void USendOneChar (u8 ch)
{
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);//μè′y·￠?í???a??
	 USART_SendData(USART2,ch); 
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);//μè′y·￠?ííê
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
void push(uchar adr,UInt16 date)
{
  uSendBuf[adr*2]=date/256;
  uSendBuf[adr*2+1]=date%256;
}


/*发送占用时间非常多，任务调度的时候一定要去掉*/
extern u8 Image_Bin[CAMERA_H][CAMERA_W];
void SendOV7620(void)
{
  	uint32_t i,j;
	  uint8_t d;
    USendOneChar(0x01);
    USendOneChar(0xFE);
		for(i = 0; i < CAMERA_H; i++)
		{ 
			for(j = 0; j < CAMERA_W; j++)
			{
				d=Image[i][j];
				//if(d > 0xFD) d = 0xFD;         
				USendOneChar(d);
			}
	  }
		USendOneChar(0xFE);
		USendOneChar(0x01);
  i=0;
  j=0;  
}







