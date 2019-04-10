#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"
#include "include.h"

#define MAX_PALSE (802) //根据电机本身来改，一圈4345个编码值，T12模式计数，所以乘4
//一圈需要


////-----------  变量状态  -------------//
//typedef struct
//{
//    int32_t pre;
//    int32_t now;
//    int32_t set;
//} INT32_T_STATE_Typedef;
//---------电机------------//  //因为用到四个电机 将名字左右改成了电机号
typedef struct
{
	INT32_T_STATE_Typedef speed;//速度（编码器周期读取）脉冲数
	
	int32_t pwm;//设定的PWM
	
	int32_t loc;//位置（速度周期累加）脉冲数
	
	int32_t round;//转过的整圈
	
	double round_total;//总共转过的圈数，精确小数
	
	double round_total_expect;//希望转到的精确圈数   亦可指位置目标值
	
	double round_total_init;//系统初始化时电机所在精确圈数
	
}MOTO_PARA_Typedef;


typedef struct
{
	MOTO_PARA_Typedef m1;
	MOTO_PARA_Typedef m2;
	MOTO_PARA_Typedef m3;
	MOTO_PARA_Typedef m4;
	
}MOTO_Typedef;



extern MOTO_Typedef moto;


void Encoder_TIM4_Init(void);
void Encoder_TIM2_Init(void);
int Read_Encoder_L(void);
int Read_Encoder_R(void);
#endif
