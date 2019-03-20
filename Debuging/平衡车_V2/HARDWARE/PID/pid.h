#ifndef __PID_H
#define __PID_H

#include "sys.h"

typedef struct PID {
    double SetPoint; // �趨Ŀ�� Desired Value
    double Proportion; // �������� Proportional Const
    double Integral; // ���ֳ��� Integral Const
    double Derivative; // ΢�ֳ��� Derivative Const
    double LastError; // �ϴ����
    double PrevError; // ��ǰ���
    double SumError; // �����,����������
    double LastError0;
    double pwmduty;
	float allowError;
} PID;

extern PID JYAngle_PID;
extern PID Speed_PID;
extern PID Encoder_Angle_PID;

void PIDCalc(PID* pp, double PreVal); //�ٶ�PID
double position_PIDCalc(PID* pp, double PreVal); //λ��PID
double PID_realize(PID* pp, double PreVal);
float PID_realize_v3(PID* pp, double PreVal);

#endif
