#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"

#define MAX_PULSE (4150*4)  //一圈需要

//读取周期请不要超过1圈，否则改一下算法
typedef struct{
		//准确的说这并不是CNT的值，而是与初始差的值,主要防止溢出
		int last_cnt_offset;    //上一次的CNT的偏差，可以作为当前角度，angle的int值  累积值
		double last_angle_offset;  //上一次的角度
		int cnt_offset;    //本次的偏差可以判断转动方向
		double angle_offset;  //cnt_offset的角度化的值
		int circle;    //这个值其实是有之前的累积的不建议,只能表明和初始比累积变化了几圈
}Encoder_Structure;


void Encoder_TIM4_Init(Encoder_Structure* Encoder_InitStructure); //里面包含结构体的初始化
double Read_Encoder(Encoder_Structure* Encoder_InitStructure);
#endif
