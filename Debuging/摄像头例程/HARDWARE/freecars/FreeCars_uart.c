#include "FreeCars_isr.h"
#include "FreeCars_uart.h"
#include "DCMI.h"
#include "OV7620.h"

/************һ���Ǹ���FreeCars��λ��Э���޸ĵģ�����ı�**************/
uchar uSendBuf[UartDataNum*2 + LineDataNum]={0};
uchar FreeCarsDataNum=UartDataNum*2; //+ LineDataNum;
//uchar FreeCarsCount=0;
double UartData[9] = {0};
SerialPortType SerialPortRx;
/**************************/


void uart2_init(u32 bound)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5����ΪUSART2 TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6����ΪUSART2 RX
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOD5��GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD5��PD6

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

// uchar UartGetOneChar(UART_MemMapPtr uartch)
uchar UartGetOneChar(void)
{
	return USART_ReceiveData(USART2);		//��������	
}

/*
����һ���ֽ�
*/
// void USendOneChar (UART_MemMapPtr uartch, uint8 ch)
void USendOneChar (u8 ch)
{
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);//�̨���y����?��???a??
	 USART_SendData(USART2,ch); 
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);//�̨���y����?������
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
void push(uchar adr,UInt16 date)
{
  uSendBuf[adr*2]=date/256;
  uSendBuf[adr*2+1]=date%256;
}


/*����ռ��ʱ��ǳ��࣬������ȵ�ʱ��һ��Ҫȥ��*/
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







