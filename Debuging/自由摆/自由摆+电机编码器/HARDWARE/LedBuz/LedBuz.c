#include "LedBuz.h"
#include "include.h"

/*   BY QH-170707
ʹ��ע�⣺
1��Ϊ�˲��÷�����һֱ�У�����������ֵbz_cntֻ����ϵͳ�л���
   ��������ģʽ��Norm��ʱ�Ż����ã���LED�ǻ��Զ����õ�
   ��д����ʱע��״̬���л���״̬����һ��Ҫ�ȹ��ȵ�Norm״̬
	 
ʹ��˵����
1����30msѭ���м���bz_led_work(SYS_State);  
2����ϵͳ1msʱ���м���bz_cnt++;led_cnt++;
3�����ctrl.c
*/

int bz_cnt=0;
int led_cnt=0;
BUZ_Enum BZ_State=BZ_Norm;
LED_Enum LED_State=LED_Norm;
SYS_Enum SYS_State=SYS_Norm;

void led_buz_Init(void)
{
	/*******��������ʼ��***************/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(macBUZ_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin=macBUZ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(macBUZ_GPIO_PORT,&GPIO_InitStructure);
	
	BUZ_OFF;
	
	/*******LED��ʼ��***************/

 macLED_AHBxClock_FUN (macLED1_GPIO_CLK, ENABLE);	 //ʹ��PC�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;				 //LED0-->PG.0 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);	   		//PG.0 �����			 
//GPIO_ResetBits(GPIOA,GPIO_Pin_6);

	macLED_AHBxClock_FUN (macLED2_GPIO_CLK, ENABLE);	 //ʹ��PC�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;				 //LED1-->PG.1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN);						 //PG.1 �����
//GPIO_ResetBits(GPIOA,GPIO_Pin_6);	
}


//================================  BUZ  ================================//

void bz_ready(void)//101010
{
	if     (bz_cnt<100)	BUZ_ON;
	else if(bz_cnt<200)  	  BUZ_OFF;
	else if(bz_cnt<300)	BUZ_ON;
	else if(bz_cnt<400)  	  BUZ_OFF;
	else if(bz_cnt<500)	BUZ_ON;
	else if(bz_cnt<600)  	  BUZ_OFF;
	if(bz_cnt>600)
	{
		BZ_State=BZ_Norm;//����
		SYS_State=SYS_Norm;
	}
}

void bz_ok(void)
{
	if     (bz_cnt<100)	BUZ_ON;
	else if(bz_cnt<200)  	  BUZ_OFF;
	else if(bz_cnt<300)	BUZ_ON;
	else if(bz_cnt<400)  	  BUZ_OFF;
	else if(bz_cnt<500)	BUZ_ON;
	else if(bz_cnt<600)  	  BUZ_OFF;
	if(bz_cnt>600)
	{
		BZ_State=BZ_Norm;//����
		SYS_State=SYS_Norm;
	}
}


//void bz_ok(void)
//{
//	if     (bz_cnt<50)	BUZ_ON;
//	else if(bz_cnt<100)  	  BUZ_OFF;
//	else if(bz_cnt<150)	BUZ_ON;
//	else if(bz_cnt<200)  	  BUZ_OFF;
//	else if(bz_cnt<250)	BUZ_ON;
//	else if(bz_cnt<300)    	BUZ_OFF;
//	
//	if(bz_cnt>300)
//	{
//		//BZ_State=BZ_Norm;
//		SYS_State=SYS_Norm;
//	}
//}

void bz_warning(void)
{
	if     (bz_cnt<800)	BUZ_ON;
	else if(bz_cnt<1000)  	  BUZ_OFF;
			
	if(bz_cnt>1000)
	{
		SYS_State=SYS_Norm;
	}
}

void bz_key(void)
{
	if     (bz_cnt<50)	BUZ_ON;
	else if(bz_cnt<100)  	  BUZ_OFF;
			
	if(bz_cnt>100)
	{
		SYS_State=SYS_Norm;
	}
}


void bz_work(int state)
{
	switch(state)
	{
		case BZ_Ready:  bz_ready();break;//bi bi
		case BZ_OK:     bz_ok();break;   //bi bi bi
		case BZ_Warning:bz_warning();break;//bi---
		case BZ_Key:    bz_key();break;

		default:
		{
			BUZ_OFF;
			bz_cnt=0;
		}
	}
}


//================================  LED  ================================//

//�̵�(1��)3s��һ�Σ���Ʋ���
void led_norm(void)
{
	if     (led_cnt<200)   { LED1(ON);LED2(OFF); }
	else if(led_cnt<3000) { LED1(OFF);LED2(OFF);}
	
	if(led_cnt>3000)
	{
		led_cnt=0;
	}
}

//����������˸
void led_warning(void)
{
	if     (led_cnt<200) { LED1(ON);  LED2(OFF);}
	else if(led_cnt<400) { LED1(OFF); LED2(ON); }
	
	if(led_cnt>400)
	{
		led_cnt=0;
	}
}


void led_ok(void)
{
	if     (led_cnt<50)   { LED1(ON);}
	else if(led_cnt<100)     { LED1(OFF);}
	else if(led_cnt<150)  { LED1(ON);}
	else if(led_cnt<200)     { LED1(OFF);}
	else if(led_cnt<250)  { LED1(ON);}
	else if(led_cnt<300)     { LED1(OFF);}
	
	if(led_cnt>300)
	{
		led_cnt=0;
	}
}

void led_work(int state)
{
	switch (state)
	{
		case LED_Norm:led_norm();break;
		case LED_Warning:led_warning();break;
		case LED_OK:led_ok();break;
		default://������
		{
			LED1(ON);
			LED2(ON);
			led_cnt=0;
		}
	}
}



//================   SYS   ================//
//BZ_State  = BZ_Normʱ �˺��������� bz_cnt����
//led ������  LED_State���ڶ���״̬ʱ��������߼�����һ��ֵ
void bz_led_work(int state)
{
	if(SYS_State==SYS_Ready)  {  BZ_State  = BZ_Ready;//0
														     LED_State = LED_Norm;}
	
	if(SYS_State==SYS_Norm )  {  BZ_State  = BZ_Norm;//1
														     LED_State = LED_Norm;}
	
	if(SYS_State==SYS_Warning){  BZ_State  = BZ_Warning;//2
														     LED_State = LED_Warning;}
	
	if(SYS_State==SYS_OK)     {  BZ_State  = BZ_OK;//3
														     LED_State = LED_OK;}
	
    if(SYS_State==SYS_Key)    {  BZ_State  = BZ_Key;
														     LED_State = LED_Norm;}
															 
	led_work(LED_State);
	bz_work(BZ_State);												 
}




