#include "RS485.h"

//RS485 ����
u8 RS485_RX_BUF[RS485_MAX_RECEIEVE]; 		//���ջ���,���
//RS485 ���ռ���
u8 RS485_RX_CNT;   			//���յ������ݳ���


//rs485ͨ�������õ���SP3485��Ӳ��ת�������ֻ��Ҫ����һ������
//Ϊ�˷���ʹ��USART2
//PA2 USART2_TX RS485_RX
//PA3 USART2_RX RS485_TX
//����Ϊ������
void rs485_init(int bound)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��GPIOA GPIOGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOG,ENABLE); 
	//ʹ��USART2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	//GPIOA3����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 	
	
	//USART2
	//GPIOA2��GPIOA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_2; 
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	//��ʼ��PA2��PA3
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	//PG8
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	//���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOG,&GPIO_InitStructure); 
	
  //USART2 ��ʼ������
	//����������
	USART_InitStructure.USART_BaudRate = bound;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	//ʹ�ܴ���2
  USART_Cmd(USART2, ENABLE);  
	//�����жϱ�־λ
	USART_ClearFlag(USART2, USART_FLAG_TC);	
	//��������ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	//Ĭ��Ϊ����ģʽ	
	RS485_RE=0;				
}

void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
	  res =USART_ReceiveData(USART2);//;��ȡ���յ�������USART2->DR
		if(RS485_RX_CNT<RS485_MAX_RECEIEVE)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
	}  											 
} 
//ֱ�Ӹ���ճ����
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_RE=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(USART2,buf[t]); //��������
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
	RS485_RX_CNT=0;	  
	RS485_RE=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}
