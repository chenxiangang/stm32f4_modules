#include "RS485.h"

//RS485 接收
u8 RS485_RX_BUF[RS485_MAX_RECEIEVE]; 		//接收缓冲,最大
//RS485 接收计数
u8 RS485_RX_CNT;   			//接收到的数据长度


//rs485通信这里用的是SP3485，硬件转换，软件只需要串口一样处理
//为了方便使用USART2
//PA2 USART2_TX RS485_RX
//PA3 USART2_RX RS485_TX
//输入为波特率
void rs485_init(int bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能GPIOA GPIOG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOG,ENABLE); 
	//使能USART2时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	//GPIOA3复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 	
	
	//USART2
	//GPIOA2与GPIOA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_2; 
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	//初始化PA2，PA3
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	//PG8
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	//输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOG,&GPIO_InitStructure); 
	
  //USART2 初始化设置
	//波特率设置
	USART_InitStructure.USART_BaudRate = bound;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	//使能串口2
  USART_Cmd(USART2, ENABLE);  
	//清理中断标志位
	USART_ClearFlag(USART2, USART_FLAG_TC);	
	//开启相关中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	//默认为接收模式	
	RS485_RE=0;				
}

void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
	  res =USART_ReceiveData(USART2);//;读取接收到的数据USART2->DR
		if(RS485_RX_CNT<RS485_MAX_RECEIEVE)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
	}  											 
} 
//直接复制粘贴了
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_RE=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART2,buf[t]); //发送数据
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
	RS485_RX_CNT=0;	  
	RS485_RE=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}
