#include "encoder.h"
//����ʹ�ö�ʱ���ı������ӿ�
//ʹ��ʱ�ǵø��жϵ����ȼ�
void Encoder_TIM4_Init(void)
{
	//ѡTIM4�о��鷳Сһ��
	//��TIM4��ת�ɱ������ӿ�
	//1,2,3,4,5,8���б������ӿ�
	//ʱ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//�жϽṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//��ʱ������ṹ��
  TIM_ICInitTypeDef TIM_ICInitStructure;
	//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	//GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	//D12,D13�˿ڸ���
	//D12 A
	//D13 B
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	//��Ҫ��©
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD,&GPIO_InitStructure); 
	//��װ��ֵ
	//��Ҫ�����ԣ��Ͼ�ֱ��65535����NMDWSM
	TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF;   //65535
  //Ԥ��Ƶϵ��	
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  //
	//���ϼ���
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	//ʱ�ӷָ�
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  //��ʼ��ʱ��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//TIM4,˫������������ģʽ�ӿ�
	 TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
	//���ö�ʱ������ṹ��ȱʡ����
	//�Ӻ����ڲ�����ͨ��1
	//��ʵ��ֻ��ͨ��1��ͨ��2�ſ���
	TIM_ICStructInit(&TIM_ICInitStructure);
	//�˲�ֵ,��ʱ��������ô�
	TIM_ICInitStructure.TIM_ICFilter = 10; 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	//�����־λ
	 TIM_ClearFlag(TIM4, TIM_FLAG_Update); 
	 //�����жϸ���
	 TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	 //��ռ���ֵ
	 TIM4->CNT=0x7FFF;     //0;
	 //�����ж�
	 NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 
	 //��ռ���ȼ�1
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	 //��Ӧ���ȼ�3
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	 //�����ж�
	 NVIC_Init(&NVIC_InitStructure);
	 
	 
	 //������ʱ��
	 TIM_Cmd(TIM4,ENABLE); 
}

void TIM4_IRQHandler(void)
{
	 if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	 {
	   //�ж����
		 //һ����˵û�м������͵���arr��ֵ��������,�˴���Ҫ��Ȧ��
		 //TODO
		 //��ʵû��
		 /**********
		 if((TIM4->CR1>>4 & 0x01)==0) //DIR==0
            circle_count++;
      else if((TIM4->CR1>>4 & 0x01)==1)//DIR==1
            circle_count--;
		 */////////
		 //printf("hello\r\n");
		 
	 }
	  TIM_ClearITPendingBit(TIM4,TIM_IT_Update); 
}
//��������ֵ
//���Ƕ�
double Read_Encoder(void)
{
		//��ȡ�Ƕ�ֵ
		static double angle=0;
	  int temp;
		int temp_circle=0;
		//int	temp_angle;
		
		temp=TIM_GetCounter(TIM4);
		temp-=0x7FFF;
		//temp_angle=temp;
		if(temp>MAX_PULSE)
		{
			temp-=MAX_PULSE;
			temp_circle++;
				//temp_circle=temp/MAX_PULSE;
				//temp_angle=temp%MAX_PULSE;
		}
		else if(temp<-MAX_PULSE)
		{
			temp+=MAX_PULSE;
			temp_circle--;
//			temp_circle=temp/MAX_PULSE;
//			temp_angle=temp%MAX_PULSE;
		}
		angle=angle+temp_circle*360+1.0*temp/MAX_PULSE*360;
		TIM4->CNT=0x7FFF;  //��λ
		return angle;
}


