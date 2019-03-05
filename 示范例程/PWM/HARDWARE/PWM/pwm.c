#include "pwm.h"

int CCR=250;
int deadtime=0;
/**************************
��������pwm_int
���ܣ���ʼ������pwm��
������arr��ʱ������ֵ��pscԤ��Ƶϵ��
**************************/
void pwm_init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_init;
	TIM_TimeBaseInitTypeDef TIM_INIT;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	#if 0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʱ��ʹ��GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//�߼���ʱ��8ʹ��
	GPIO_init.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//GPIOB��0��1�� GPIOB0ΪTIM8_CH2N,GPIOB1ΪIM8_CH3N
	GPIO_init.GPIO_Mode=GPIO_Mode_AF; //���ù���
	GPIO_init.GPIO_Speed=GPIO_Speed_100MHz; //����
	GPIO_init.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_init.GPIO_PuPd=GPIO_PuPd_UP; //������
	GPIO_Init(GPIOB,&GPIO_init);  //��ʼ��GPIOB0 GPIOB1
	GPIO_init.GPIO_Pin=GPIO_Pin_7;//GPIOA7ΪTIM8_CH1N
	GPIO_Init(GPIOA,&GPIO_init); 
	GPIO_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;     //GPIOC6ΪTIM8_CH1 TIM8_CH2
	GPIO_Init(GPIOC,&GPIO_init); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM8); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);//��������
	#endif
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//�߼���ʱ��8ʹ��
	GPIO_init.GPIO_Pin=GPIO_Pin_7;
	GPIO_init.GPIO_Mode=GPIO_Mode_AF; //���ù���
	GPIO_init.GPIO_Speed=GPIO_Speed_100MHz; //����
	GPIO_init.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_init.GPIO_PuPd=GPIO_PuPd_UP; //������
	GPIO_Init(GPIOA,&GPIO_init);    //GPIOA7
	GPIO_init.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOC,&GPIO_init);   //GPIOC6
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	TIM_INIT.TIM_ClockDivision=TIM_CKD_DIV1; //1��Ƶ
	TIM_INIT.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_INIT.TIM_Period=arr;  //�Զ����ؼĴ�������ֵ
	TIM_INIT.TIM_Prescaler=psc;   //Ԥ��Ƶϵ��
	TIM_INIT.TIM_RepetitionCounter=1; //���ڼ���ֵ
	TIM_TimeBaseInit(TIM8,&TIM_INIT);  
  TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;//��MOE=0�������
  TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //�����ȵ���ģʽΪ1
  TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//��moe=0�Ƚ���������Ƚ�N����״̬
  TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;// ��������Ƚϼ��Ը�
  TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//����Ƚϼ��Ը�
  TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//ʹ������Ƚ�״̬
  TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ�������Ƚ�״̬
  TIM_OCInitStruct.TIM_Pulse=CCR;	//��װ�벶��ȽϼĴ���������ֵ  �Ƚ�ֵ
  TIM_OC1Init(TIM8,&TIM_OCInitStruct);
 // TIM_OC2Init(TIM8,&TIM_OCInitStruct);
 // TIM_OC3Init(TIM8,&TIM_OCInitStruct);//����TIM8_OCinitStruct��ָ���Ĳ�����ʼ�� TIM8ͨ��3
//ʹ�ܶ˴�
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	//TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//ʹ��TIM8��CCR�ϵ�Ԥװ�س�ֵ
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable; //�Զ����ʹ��
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable; //ɲ��ʹ��
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High; //ɲ������̼ܽ��Ը�
	TIM_BDTRInitStruct.TIM_DeadTime=deadtime;  //��������
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_OFF;  //����ƽ�����������κ�λ
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Disable;  //����������ģʽ�·ǹ���״̬ѡ��
	TIM_BDTRInitStruct.TIM_OSSRState= TIM_OSSRState_Disable;  //����������ģʽ�·ǹ���״̬ѡ��
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);
	TIM_ARRPreloadConfig(TIM8,ENABLE);  //ʹ��TIM8��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM8,ENABLE); //pwmʹ����� �߼���ʱ����
	TIM_Cmd(TIM8,ENABLE);       //��TIM8
}
