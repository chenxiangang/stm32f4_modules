#include "tb6612.h"

void TB6612_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		
		//����GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
}

void TIM3_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// 
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);//ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=2100-1;  //��װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=4-1;  //Ԥ��Ƶϵ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWMģʽ1 ����CCRΪ��Ч����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��Ч����Ϊ��
	TIM_OCInitStructure.TIM_Pulse=0;    //��ʼCRRΪ0  ����ʼ��ת
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);   //CH1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);   //CH2
	
	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ   ���µ���ʱ���ܸ�CCR��������ʱ��
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ
	TIM_ARRPreloadConfig(TIM3,ENABLE);//������ʱ��ARR  ���µ������ܸ�
 
}

void TB6612_Init()
{
	TB6612_init();
	TIM3_PWM_Init();
	TIM_Cmd(TIM3, ENABLE);  //����ʱ��
}

void speedcontrl(int speed,int ch)
{
		if(speed > 0 && ch == 1)
		{
			AIN1 = 1;
			AIN2 = 0;
			TIM_SetCompare1(TIM3,speed);
		}
		else if(speed < 0 && ch == 1)
		{
			AIN1 = 0;
			AIN2 = 1;
			TIM_SetCompare1(TIM3,speed);
		}
		else if(speed > 0 && ch == 2)
		{
		  BIN1 = 1;
			BIN2 = 0;
			TIM_SetCompare2(TIM3,speed);
		}
		else if(speed < 0 && ch == 2)
		{
			BIN1 = 0;
			BIN2 = 1;
			TIM_SetCompare2(TIM3,speed);
		}
		 
}
