/*===========================================================
文件名称：JY901_uart.c

修改人：QH

修改日期：2017/5/11

引脚设置：使用串口3中断

使用说明：使用时将JY901_uart.c,JY901_uart.h共同添加进工程
					需要初始化串口3，使能串口3中断
					需要配置串口3中断优先级，建议优先级较高
					周期大概5ms，实际串口周期和读取时间未测
					无需外部调用此文件的函数
		
===========================================================*/

#include "JY901_uart.h"
#include "include.h"
#include "LedBuz.h"
#include "delay.h"



XYZ_Typedef acc;//加速度
XYZ_Typedef gryo;//角速度
XYZ_Typedef magnetic;//磁场	-->未读取


ANGLE_DEAL_Typedef angle;

float  pitch;
float roll;
float yaw;


void usart3_init(unsigned int bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; //PA2,PA3,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA2，PA3
	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA3复用为USART2
  
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =  USART_Mode_Rx;	// 收模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
	
  USART_Cmd(USART3, ENABLE);  //使能串口

	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

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

//CopeSerialData为串口3中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
		  //加速度
			case 0x51:	
			{
					acc.x=(float)CharToShort(&ucRxBuffer[2])/32768*16*9.8;
					acc.y=(float)CharToShort(&ucRxBuffer[4])/32768*16*9.8;
					acc.z=(float)CharToShort(&ucRxBuffer[6])/32768*16*9.8;
					break;
			}
			
			//角速度
			case 0x52:	
			{
					gryo.x=(float)CharToShort(&ucRxBuffer[2])/32768*500;
					gryo.y=(float)CharToShort(&ucRxBuffer[4])/32768*500;
					gryo.z=(float)CharToShort(&ucRxBuffer[6])/32768*500;
					break;
			}
			//角度
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

/*pitch  roll 角度融合  以适应放歪的情况*/
float mixture_angle(float pitch,float roll)
{
	if(roll>0)//pitch>0&&  不合理  暂时这么用
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
		CopeSerial2Data((unsigned char)USART3->DR);//处理数据	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
   }
}




/* 初始化，测量误差 */
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




