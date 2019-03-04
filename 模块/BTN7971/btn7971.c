#include "btn7971.h"


//channel ѡ��ͨ��
//1ΪEN1 2λEN2 3Ϊȫ��
//����Ӳ���ϽӸߵ�ƽ��ִ���������
void BTN7971_EN_Init(int channel)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		//����GPIOA
		RCC_AHB1PeriphClockCmd(BTN7971_EN1_RCC_GPIO|BTN7971_EN2_RCC_GPIO,ENABLE); 

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	
		if(1==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN1_PIN;
		else if(2==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN2_PIN;
		else if(3==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN1_PIN|BTN7971_EN2_PIN;
		
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
		
		//Ĭ�Ͽ���
		if(1==channel)
			BTN7971_EN1=1;
		else if(2==channel)
			BTN7971_EN2=1;
		else if(3==channel)
		{
			BTN7971_EN1=1;
			BTN7971_EN2=1;
		}
}
//������һ����
void BTN7971_EN1_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(BTN7971_EN1_IN1_RCC_GPIO|BTN7971_EN1_IN2_RCC_GPIO,ENABLE);
	RCC_APB1PeriphClockCmd(BTN7971_TIM_RCC, ENABLE);//��ʱ��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch1;

	GPIO_Init(BTN7971_EN1_IN1_GPIO, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch2;
	
	GPIO_Init(BTN7971_EN1_IN2_GPIO, &GPIO_InitStructure);//��ʼ��
	//����
	GPIO_PinAFConfig(BTN7971_EN1_IN1_GPIO,BTN7971_EN1_IN1_GPIOPINSOURCE,BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN1_IN2_GPIO,BTN7971_EN1_IN2_GPIOPINSOURCE,BTN7971_TIM_AF);
	
	//ʱ����ʼ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=BTN7971_ARR;  //��װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=BTN7971_PSC;  //Ԥ��Ƶϵ��
	
	TIM_TimeBaseInit(BTN7971_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWMģʽ1 ����CCRΪ��Ч����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��Ч����Ϊ��
	TIM_OCInitStructure.TIM_Pulse=0;    //��ʼCRRΪ0  ����ʼ��ת
	
	TIM_OC1Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH1
	TIM_OC2Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH2
	
	TIM_OC1PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ   ���µ���ʱ���ܸ�CCR��������ʱ��
	TIM_OC2PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ
	TIM_ARRPreloadConfig(BTN7971_TIM,ENABLE);//������ʱ��ARR  ���µ������ܸ�
}

//�ڶ�����
void BTN7971_EN2_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(BTN7971_EN2_IN1_RCC_GPIO|BTN7971_EN2_IN2_RCC_GPIO,ENABLE);
	RCC_APB1PeriphClockCmd(BTN7971_TIM_RCC, ENABLE);//��ʱ��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch3;

	GPIO_Init(BTN7971_EN2_IN1_GPIO, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch4;
	
	GPIO_Init(BTN7971_EN2_IN2_GPIO, &GPIO_InitStructure);//��ʼ��
	//����
	GPIO_PinAFConfig(BTN7971_EN2_IN1_GPIO,BTN7971_EN2_IN1_GPIOPINSOURCE,BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN2_IN2_GPIO,BTN7971_EN2_IN2_GPIOPINSOURCE,BTN7971_TIM_AF);
	
	//ʱ����ʼ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=BTN7971_ARR;  //��װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=BTN7971_PSC;  //Ԥ��Ƶϵ��
	
	TIM_TimeBaseInit(BTN7971_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWMģʽ1 ����CCRΪ��Ч����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��Ч����Ϊ��
	TIM_OCInitStructure.TIM_Pulse=0;    //��ʼCRRΪ0  ����ʼ��ת
	
	TIM_OC3Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH3
	TIM_OC4Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH4
	
	TIM_OC3PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ   ���µ���ʱ���ܸ�CCR��������ʱ��
	TIM_OC4PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //ʹ��Ԥװ�س�ֵ
	TIM_ARRPreloadConfig(BTN7971_TIM,ENABLE);//������ʱ��ARR  ���µ������ܸ�
}


//��ʼ��
void BTN7971_Init(void)
{
		BTN7971_EN_Init(3);   //��Ӳ�������Ƿ���Ҫ
		BTN7971_EN1_PWM_Init();  //ʹ��1
		BTN7971_EN2_PWM_Init();		//ʹ��2
		TIM_Cmd(BTN7971_TIM, ENABLE);  //����ʱ��
}
//�ٶȿ���
//speed=CCR
//chѡ��ͨ��   1ΪEN1 ��2ΪEN2
void contrl_speed(int speed,int ch)
{
	if(speed>MAX_SPEED)
		speed=MAX_SPEED;  //�޷�
	else if(speed <(-MAX_SPEED))
		speed=(-MAX_SPEED);
	if(speed>0)    //��ת
	{	
		if(1==ch)
		{
			TIM_SetCompare1(BTN7971_TIM,speed);   //EN1  IN1 PWM
			TIM_SetCompare2(BTN7971_TIM,0);       //EN2   IN2 0
		}
		else if(2==ch)
		{
			TIM_SetCompare3(BTN7971_TIM,speed); //EN2   IN1  PWM
			TIM_SetCompare4(BTN7971_TIM,0);     //EN2    IN2 0
		}
	}
	else   //��ת
	{
			speed=-speed;    //�Ȼ�����
			if(1==ch)
		{
			TIM_SetCompare1(BTN7971_TIM,0);   //EN1  IN1 0
			TIM_SetCompare2(BTN7971_TIM,speed);       //EN2   IN2 PWM
		}
		else if(2==ch)
		{
			TIM_SetCompare3(BTN7971_TIM,0); //EN2   IN1  0
			TIM_SetCompare4(BTN7971_TIM,speed);     //EN2    IN2 0
		}
	}
}
