#include "encoder.h"

MOTO_Typedef moto;

//尝试使用定时器的编码器接口
//使用时记得改中断的优先级
void Encoder_TIM4_Init(void)
{
    //选TIM4感觉麻烦小一点
    //将TIM4包转成编码器接口
    //1,2,3,4,5,8才有编码器接口
    //时基结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //GPIO结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    //定时器输入结构体
    TIM_ICInitTypeDef TIM_ICInitStructure;
    //开启TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    //D12,D13端口复用
    //D12 A
    //D13 B
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    //主要开漏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		TIM_DeInit(TIM4);
    //重装载值
    //不要都可以，毕竟直接65535走起，NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
    //预分频系数
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //向上计数
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //时钟分割
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //初始化时基
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    //TIM4,双计数，编码器模式接口
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //配置定时器输入结构体缺省部分
    //从函数内部看是通道1
    //事实上只有通道1和通道2才可以
    TIM_ICStructInit(&TIM_ICInitStructure);
    //滤波值,暂时还不清楚用处
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    //清除标志位
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //允许中断更新
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //清空计数值
    TIM4->CNT = 0x7FFF; //0;

    //开启定时器
    TIM_Cmd(TIM4, ENABLE);
}

void Encoder_TIM2_Init(void)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
    //主要开漏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		TIM_DeInit(TIM2);
    //重装载值
    //不要都可以，毕竟直接65535走起，NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
    //预分频系数
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //向上计数
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //时钟分割
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //初始化时基
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    //TIM4,双计数，编码器模式接口
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //配置定时器输入结构体缺省部分
    //从函数内部看是通道1
    //事实上只有通道1和通道2才可以
    TIM_ICStructInit(&TIM_ICInitStructure);
    //滤波值,暂时还不清楚用处
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    //清除标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    //允许中断更新
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM2->CNT = 0x7FFF; //0;

    TIM_Cmd(TIM2, ENABLE);
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
        //中断溢出
        //一般来说没有计数满就到了arr的值会产生溢出,此处需要计圈数
    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        //中断溢出
        //一般来说没有计数满就到了arr的值会产生溢出,此处需要计圈数
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
//读编码器值
//读角度
int Read_Encoder_L(void)
{
//    int temp;
//    temp = TIM_GetCounter(TIM4);
//    temp -= 0x7FFF;
//    TIM4->CNT = 0x7FFF; //复位
//    return temp;
	moto.m1.speed.now=TIM_GetCounter(TIM4)-0x7fff;
	moto.m1.loc += moto.m1.speed.now;
	if(moto.m1.loc>MAX_PALSE)//超过最大脉冲数
	{
		moto.m1.round++;//整数圈数+1
		moto.m1.loc-=MAX_PALSE;//电机位置更新（电机位置不超过最大脉冲数）
	}
	if(moto.m1.loc<-MAX_PALSE)//反转
	{
		moto.m1.round--;//整圈圈数-1
		moto.m1.loc+=MAX_PALSE;//电机位置更新（电机位置不为负数）
	}
	moto.m1.round_total=moto.m1.round + (double)moto.m1.loc/MAX_PALSE;//计算精确圈数
	TIM4->CNT=0x7fff;
	return moto.m1.speed.now;
}
int Read_Encoder_R(void)
{  
//    int temp;
//    temp = TIM_GetCounter(TIM2);
//    temp -= 0x7FFF;
//    TIM2->CNT = 0x7FFF; //复位
//    return temp;
	//此处默认左右电机转一圈脉冲数相同	
	//右边电机
	moto.m2.speed.now=TIM_GetCounter(TIM2)-0x7fff;
	moto.m2.loc+=moto.m2.speed.now;
	if(moto.m2.loc>MAX_PALSE)
	{
		moto.m2.round++;
		moto.m2.loc-=MAX_PALSE;
	}
	if(moto.m2.loc<-MAX_PALSE)
	{
		moto.m2.round--;
		moto.m2.loc+=MAX_PALSE;
	}
	moto.m2.round_total=moto.m2.round + (double)moto.m2.loc/MAX_PALSE;
 	TIM2->CNT=0x7fff;
	return moto.m2.speed.now;
}
