/*===========================================================
�ļ����ƣ�JY901_uart.c

�޸��ˣ�QH

�޸����ڣ�2017/5/11

�������ã�ʹ�ô���3�ж�

ʹ��˵����ʹ��ʱ��JY901_uart.c,JY901_uart.h��ͬ��ӽ�����
					��Ҫ��ʼ������3��ʹ�ܴ���3�ж�
					��Ҫ���ô���3�ж����ȼ����������ȼ��ϸ�
					���ڴ��5ms��ʵ�ʴ������ںͶ�ȡʱ��δ��
					�����ⲿ���ô��ļ��ĺ���
		
===========================================================*/

#include "JY901_uart.h"
#include "include.h"
#include "LedBuz.h"
#include "delay.h"



XYZ_Typedef acc;//���ٶ�
XYZ_Typedef gryo;//���ٶ�
XYZ_Typedef magnetic;//�ų�	-->δ��ȡ


ANGLE_DEAL_Typedef angle;

float  pitch;
float roll;
float yaw;


void usart3_init(unsigned int bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; //PA2,PA3,���ù���,�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA3����ΪUSART2
  
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode =  USART_Mode_Rx;	// ��ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������
	
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���

	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

}

void ShortToChar(short sData,unsigned char cData[])
{
	cData[0]=sData&0xff;
	cData[1]=sData>>8;
}

short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8)|cData[0];
}

//CopeSerialDataΪ����3�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
		  //���ٶ�
			case 0x51:	
			{
					acc.x=(float)CharToShort(&ucRxBuffer[2])/32768*16*9.8;
					acc.y=(float)CharToShort(&ucRxBuffer[4])/32768*16*9.8;
					acc.z=(float)CharToShort(&ucRxBuffer[6])/32768*16*9.8;
					break;
			}
			
			//���ٶ�
			case 0x52:	
			{
					gryo.x=(float)CharToShort(&ucRxBuffer[2])/32768*500;
					gryo.y=(float)CharToShort(&ucRxBuffer[4])/32768*500;
					gryo.z=(float)CharToShort(&ucRxBuffer[6])/32768*500;
					break;
			}
			//�Ƕ�
			case 0x53:	
			{
				roll=  (float)CharToShort(&ucRxBuffer[2])/32768*180;
				pitch= (float)CharToShort(&ucRxBuffer[4])/32768*180;
				yaw =   (float)CharToShort(&ucRxBuffer[6])/32768*180;
				break;
			}
		}
		ucRxCnt=0;
	}
}

/*pitch  roll �Ƕ��ں�  ����Ӧ��������*/
float mixture_angle(float pitch,float roll)
{
	if(roll>0)//pitch>0&&  ������  ��ʱ��ô��
		return R2A*asin(my_sqrt( my_pow( sin( pitch*A2R ) ,2 ) + my_pow( sin( roll*A2R ) ,2 ) ));
	else if(roll<0)//pitch<0||
		return -R2A*asin(my_sqrt( my_pow( sin( pitch*A2R ) ,2 ) + my_pow( sin( roll*A2R ) ,2 ) ));
	else
		return 0;
}



void USART3_IRQHandler(void)
{
	 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((unsigned char)USART3->DR);//��������	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
   }
}




/* ��ʼ����������� */
void JY901_offset(int times)
{
	int i;
	
	delay_ms(1000);
	for(i=0;i<times;i++)
	{
		delay_ms(10);
		angle.pitch.offset += pitch;
		angle.roll.offset  += roll;
		angle.yaw.offset   += yaw;
	}
	
	angle.pitch.offset /=times;
	angle.roll.offset  /=times;
	angle.yaw.offset   /=times;
	
	//SYS_State = SYS_OK;
	flag.JY901_offset_OK = 1;
}




