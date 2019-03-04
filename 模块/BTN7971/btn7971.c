#include "btn7971.h"


//channel 选择通道
//1为EN1 2位EN2 3为全开
//可以硬件上接高电平不执行这个函数
void BTN7971_EN_Init(int channel)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		//开启GPIOA
		RCC_AHB1PeriphClockCmd(BTN7971_EN1_RCC_GPIO|BTN7971_EN2_RCC_GPIO,ENABLE); 

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
		if(1==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN1_PIN;
		else if(2==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN2_PIN;
		else if(3==channel)
			GPIO_InitStructure.GPIO_Pin=BTN7971_EN1_PIN|BTN7971_EN2_PIN;
		
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
		
		//默认开启
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
//开启第一组电机
void BTN7971_EN1_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(BTN7971_EN1_IN1_RCC_GPIO|BTN7971_EN1_IN2_RCC_GPIO,ENABLE);
	RCC_APB1PeriphClockCmd(BTN7971_TIM_RCC, ENABLE);//定时器时钟使能
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch1;

	GPIO_Init(BTN7971_EN1_IN1_GPIO, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch2;
	
	GPIO_Init(BTN7971_EN1_IN2_GPIO, &GPIO_InitStructure);//初始化
	//复用
	GPIO_PinAFConfig(BTN7971_EN1_IN1_GPIO,BTN7971_EN1_IN1_GPIOPINSOURCE,BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN1_IN2_GPIO,BTN7971_EN1_IN2_GPIOPINSOURCE,BTN7971_TIM_AF);
	
	//时基初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=BTN7971_ARR;  //重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler=BTN7971_PSC;  //预分频系数
	
	TIM_TimeBaseInit(BTN7971_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWM模式1 低于CCR为有效极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效极性为高
	TIM_OCInitStructure.TIM_Pulse=0;    //初始CRR为0  即开始不转
	
	TIM_OC1Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH1
	TIM_OC2Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH2
	
	TIM_OC1PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
	TIM_OC2PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值
	TIM_ARRPreloadConfig(BTN7971_TIM,ENABLE);//允许工作时改ARR  更新到来才能改
}

//第二组电机
void BTN7971_EN2_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(BTN7971_EN2_IN1_RCC_GPIO|BTN7971_EN2_IN2_RCC_GPIO,ENABLE);
	RCC_APB1PeriphClockCmd(BTN7971_TIM_RCC, ENABLE);//定时器时钟使能
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch3;

	GPIO_Init(BTN7971_EN2_IN1_GPIO, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin=BTN7971_TIM_ch4;
	
	GPIO_Init(BTN7971_EN2_IN2_GPIO, &GPIO_InitStructure);//初始化
	//复用
	GPIO_PinAFConfig(BTN7971_EN2_IN1_GPIO,BTN7971_EN2_IN1_GPIOPINSOURCE,BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN2_IN2_GPIO,BTN7971_EN2_IN2_GPIOPINSOURCE,BTN7971_TIM_AF);
	
	//时基初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=BTN7971_ARR;  //重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler=BTN7971_PSC;  //预分频系数
	
	TIM_TimeBaseInit(BTN7971_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;    //PWM模式1 低于CCR为有效极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效极性为高
	TIM_OCInitStructure.TIM_Pulse=0;    //初始CRR为0  即开始不转
	
	TIM_OC3Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH3
	TIM_OC4Init(BTN7971_TIM, &TIM_OCInitStructure);   //CH4
	
	TIM_OC3PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
	TIM_OC4PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值
	TIM_ARRPreloadConfig(BTN7971_TIM,ENABLE);//允许工作时改ARR  更新到来才能改
}


//初始化
void BTN7971_Init(void)
{
		BTN7971_EN_Init(3);   //看硬件需求是否需要
		BTN7971_EN1_PWM_Init();  //使能1
		BTN7971_EN2_PWM_Init();		//使能2
		TIM_Cmd(BTN7971_TIM, ENABLE);  //开启时钟
}
//速度控制
//speed=CCR
//ch选择通道   1为EN1 ，2为EN2
void contrl_speed(int speed,int ch)
{
	if(speed>MAX_SPEED)
		speed=MAX_SPEED;  //限幅
	else if(speed <(-MAX_SPEED))
		speed=(-MAX_SPEED);
	if(speed>0)    //正转
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
	else   //翻转
	{
			speed=-speed;    //先换成正
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
