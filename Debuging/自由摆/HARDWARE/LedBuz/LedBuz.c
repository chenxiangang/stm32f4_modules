#include "LedBuz.h"
#include "include.h"

/*   BY QH-170707
使用注意：
1、为了不让蜂鸣器一直叫，蜂鸣器计数值bz_cnt只有在系统切换到
   正常工作模式（Norm）时才会重置，而LED是会自动重置的
   编写代码时注意状态的切换，状态过度一定要先过度到Norm状态
	 
使用说明：
1、在30ms循环中加入bz_led_work(SYS_State);  
2、在系统1ms时基中加入bz_cnt++;led_cnt++;
3、详见ctrl.c
*/

int bz_cnt=0;
int led_cnt=0;
BUZ_Enum BZ_State=BZ_Norm;
LED_Enum LED_State=LED_Norm;
SYS_Enum SYS_State=SYS_Norm;

void led_buz_Init(void)
{
	/*******蜂鸣器初始化***************/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(macBUZ_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin=macBUZ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(macBUZ_GPIO_PORT,&GPIO_InitStructure);
	
	BUZ_OFF;
	
	/*******LED初始化***************/

 macLED_AHBxClock_FUN (macLED1_GPIO_CLK, ENABLE);	 //使能PC端口时钟
 GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;				 //LED0-->PG.0 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);	   		//PG.0 输出高			 
//GPIO_ResetBits(GPIOA,GPIO_Pin_6);

	macLED_AHBxClock_FUN (macLED2_GPIO_CLK, ENABLE);	 //使能PC端口时钟
 GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;				 //LED1-->PG.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN);						 //PG.1 输出高
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
		BZ_State=BZ_Norm;//不响
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
		BZ_State=BZ_Norm;//不响
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

//绿灯(1号)3s闪一次，红灯不亮
void led_norm(void)
{
	if     (led_cnt<200)   { LED1(ON);LED2(OFF); }
	else if(led_cnt<3000) { LED1(OFF);LED2(OFF);}
	
	if(led_cnt>3000)
	{
		led_cnt=0;
	}
}

//警报交替闪烁
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
		default://出问题
		{
			LED1(ON);
			LED2(ON);
			led_cnt=0;
		}
	}
}



//================   SYS   ================//
//BZ_State  = BZ_Norm时 此函数结束后 bz_cnt归零
//led 出问题  LED_State不在定义状态时才清零或者计数到一定值
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




