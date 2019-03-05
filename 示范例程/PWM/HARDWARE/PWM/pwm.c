#include "pwm.h"

int CCR=250;
int deadtime=0;
/**************************
函数名：pwm_int
功能：初始化互补pwm波
参数：arr定时器周期值，psc预分频系数
**************************/
void pwm_init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_init;
	TIM_TimeBaseInitTypeDef TIM_INIT;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	#if 0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //时钟使能GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//高级定时器8使能
	GPIO_init.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//GPIOB的0和1口 GPIOB0为TIM8_CH2N,GPIOB1为IM8_CH3N
	GPIO_init.GPIO_Mode=GPIO_Mode_AF; //复用功能
	GPIO_init.GPIO_Speed=GPIO_Speed_100MHz; //高速
	GPIO_init.GPIO_OType=GPIO_OType_PP; //推挽输出
	GPIO_init.GPIO_PuPd=GPIO_PuPd_UP; //上拉？
	GPIO_Init(GPIOB,&GPIO_init);  //初始化GPIOB0 GPIOB1
	GPIO_init.GPIO_Pin=GPIO_Pin_7;//GPIOA7为TIM8_CH1N
	GPIO_Init(GPIOA,&GPIO_init); 
	GPIO_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;     //GPIOC6为TIM8_CH1 TIM8_CH2
	GPIO_Init(GPIOC,&GPIO_init); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM8); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);//复用连接
	#endif
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//高级定时器8使能
	GPIO_init.GPIO_Pin=GPIO_Pin_7;
	GPIO_init.GPIO_Mode=GPIO_Mode_AF; //复用功能
	GPIO_init.GPIO_Speed=GPIO_Speed_100MHz; //高速
	GPIO_init.GPIO_OType=GPIO_OType_PP; //推挽输出
	GPIO_init.GPIO_PuPd=GPIO_PuPd_UP; //上拉？
	GPIO_Init(GPIOA,&GPIO_init);    //GPIOA7
	GPIO_init.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOC,&GPIO_init);   //GPIOC6
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	TIM_INIT.TIM_ClockDivision=TIM_CKD_DIV1; //1分频
	TIM_INIT.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_INIT.TIM_Period=arr;  //自动重载寄存器周期值
	TIM_INIT.TIM_Prescaler=psc;   //预分频系数
	TIM_INIT.TIM_RepetitionCounter=1; //周期计数值
	TIM_TimeBaseInit(TIM8,&TIM_INIT);  
  TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;//当MOE=0重置输出
  TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //脉冲宽度调制模式为1
  TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//当moe=0比较重置输出比较N空闲状态
  TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;// 互补输出比较极性高
  TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//输出比较极性高
  TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//使能输出比较状态
  TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;//使能互补输出比较状态
  TIM_OCInitStruct.TIM_Pulse=CCR;	//待装入捕获比较寄存器的脉冲值  比较值
  TIM_OC1Init(TIM8,&TIM_OCInitStruct);
 // TIM_OC2Init(TIM8,&TIM_OCInitStruct);
 // TIM_OC3Init(TIM8,&TIM_OCInitStruct);//根据TIM8_OCinitStruct中指定的参数初始化 TIM8通道3
//使能端打开
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	//TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//使能TIM8在CCR上的预装载初值
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable; //自动输出使能
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable; //刹车使能
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High; //刹车输入管教极性高
	TIM_BDTRInitStruct.TIM_DeadTime=deadtime;  //死区设置
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_OFF;  //锁电平参数，不锁任何位
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Disable;  //设置在运行模式下非工作状态选项
	TIM_BDTRInitStruct.TIM_OSSRState= TIM_OSSRState_Disable;  //设置在运行模式下非工作状态选项
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);
	TIM_ARRPreloadConfig(TIM8,ENABLE);  //使能TIM8在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM8,ENABLE); //pwm使能输出 高级定时器用
	TIM_Cmd(TIM8,ENABLE);       //打开TIM8
}
