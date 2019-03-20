#ifndef __PID_H
#define __PID_H

#include "sys.h"

typedef struct PID {
    double SetPoint; // 设定目标 Desired Value
    double Proportion; // 比例常数 Proportional Const
    double Integral; // 积分常数 Integral Const
    double Derivative; // 微分常数 Derivative Const
    double LastError; // 上次误差
    double PrevError; // 当前误差
    double SumError; // 总误差,用来做积分
    double LastError0;
    double pwmduty;
	float allowError;
} PID;

extern PID JYAngle_PID;
extern PID Speed_PID;
extern PID Encoder_Angle_PID;

void PIDCalc(PID* pp, double PreVal); //速度PID
double position_PIDCalc(PID* pp, double PreVal); //位置PID
double PID_realize(PID* pp, double PreVal);
float PID_realize_v3(PID* pp, double PreVal);

#endif
