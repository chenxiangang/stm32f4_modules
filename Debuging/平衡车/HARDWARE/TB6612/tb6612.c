#include "tb6612.h"

void TB6612_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		
		//开启GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
}

void TIM3_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// 
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);//使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=2100-1;  //重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler=4-1;  //预分频系数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWM模式1 低于CCR为有效极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效极性为高
	TIM_OCInitStructure.TIM_Pulse=0;    //初始CRR为0  即开始不转
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);   //CH1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);   //CH2
	
	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载初值
	TIM_ARRPreloadConfig(TIM3,ENABLE);//允许工作时改ARR  更新到来才能改
 
}

void TB6612_Init()
{
	TB6612_init();
	TIM3_PWM_Init();
	TIM_Cmd(TIM3, ENABLE);  //开启时钟
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
