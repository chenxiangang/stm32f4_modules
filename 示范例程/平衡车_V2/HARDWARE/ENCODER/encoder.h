#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"
#include "include.h"

#define MAX_PALSE (802) //���ݵ���������ģ�һȦ4345������ֵ��T12ģʽ���������Գ�4
//һȦ��Ҫ


////-----------  ����״̬  -------------//
//typedef struct
//{
//    int32_t pre;
//    int32_t now;
//    int32_t set;
//} INT32_T_STATE_Typedef;
//---------���------------//  //��Ϊ�õ��ĸ���� ���������Ҹĳ��˵����
typedef struct
{
	INT32_T_STATE_Typedef speed;//�ٶȣ����������ڶ�ȡ��������
	
	int32_t pwm;//�趨��PWM
	
	int32_t loc;//λ�ã��ٶ������ۼӣ�������
	
	int32_t round;//ת������Ȧ
	
	double round_total;//�ܹ�ת����Ȧ������ȷС��
	
	double round_total_expect;//ϣ��ת���ľ�ȷȦ��   ���ָλ��Ŀ��ֵ
	
	double round_total_init;//ϵͳ��ʼ��ʱ������ھ�ȷȦ��
	
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
