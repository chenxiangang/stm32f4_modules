#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (4150*4)  //һȦ��Ҫ

//��ȡ�����벻Ҫ����1Ȧ�������һ���㷨
typedef struct{
		//׼ȷ��˵�Ⲣ����CNT��ֵ���������ʼ���ֵ,��Ҫ��ֹ���
		int last_cnt_offset;    //��һ�ε�CNT��ƫ�������Ϊ��ǰ�Ƕȣ�angle��intֵ  �ۻ�ֵ
		double last_angle_offset;  //��һ�εĽǶ�
		int cnt_offset;    //���ε�ƫ������ж�ת������
		double angle_offset;  //cnt_offset�ĽǶȻ���ֵ
		int circle;    //���ֵ��ʵ����֮ǰ���ۻ��Ĳ�����,ֻ�ܱ����ͳ�ʼ���ۻ��仯�˼�Ȧ
}Encoder_Structure;


void Encoder_TIM4_Init(Encoder_Structure* Encoder_InitStructure); //��������ṹ��ĳ�ʼ��
double Read_Encoder(Encoder_Structure* Encoder_InitStructure);
#endif
