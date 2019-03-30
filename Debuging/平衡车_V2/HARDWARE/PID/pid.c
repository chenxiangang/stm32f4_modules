#include "pid.h"
#include "math.h"

PID JYAngle_PID; //����JY901���������ݽ��п���
PID Encoder_Angle_PID; //����������⵽�ĽǶ���Ϊ������
PID Speed_PID; //���õ���ٶ�������
PID Turn_PID;  //ת�ٻ�

//����pid�ṹ�������������ط�����,�������pwm���
void PIDCalc(PID* pp, double PreVal) // PreErrorΪ��ǰ���ֵ
{
    double dutychange;
    double dError;
    double tempP;
    pp->PrevError = pp->SetPoint - PreVal;

    //���趨��������Χ�ڲ��ٽ��е���
    if (pp->PrevError < pp->allowError && pp->PrevError > -pp->allowError) {
        pp->PrevError = 0;
        pp->SumError = 0;
        pp->LastError = 0;
    }
    //���Էֶν���P������
    tempP = pp->Proportion;

    //��һ����Χ�ڲſ�ʼ����
    if (pp->PrevError < pp->allowError * 6 && pp->PrevError > -pp->allowError * 6)
        pp->SumError = pp->SumError + pp->PrevError; // ���֣���ʷƫ���ۼ�
    else
        pp->SumError = 0;

    dError = pp->PrevError - pp->LastError; // ΢�֣�ƫ�����
    pp->LastError = pp->PrevError;

    dutychange = tempP * pp->PrevError + pp->Integral * pp->SumError + pp->Derivative * dError;

    //�޷�
    // if (dutychange > 800)
    //     dutychange = 800;
    // else if (dutychange < -800)
    //     dutychange = -800;
    // else {
    // }
    pp->pwmduty = dutychange;
}

//double position_PIDCalc(PID* pp, double PreVal) // PreErrorΪ��ǰ���ֵ  λ��PID
//{
//    double dutychange;
//    double dError;
//	  double tempP;
//
//    pp->PrevError = PreVal - pp->SetPoint;

//
//    pp->SumError = pp->SumError + pp->PrevError; // ���֣���ʷƫ���ۼ�
//    dError = pp->PrevError - pp->LastError; // ΢�֣�ƫ�����,�ĸ����ĸ�?
//    pp->LastError = pp->PrevError;

//    dutychange = pp->Proportion * pp->PrevError + pp->Integral * pp->SumError + pp->Derivative * dError;

//    return (dutychange);
//}

//double PID_realize(PID* pp, double PreVal){
//	double dutychange;
//	double tempP;
// pp->PrevError = pp->SetPoint - PreVal;
//	if(pp->PrevError < 3.5 && pp->PrevError > -3.5)
//		pp->PrevError = 0;
//	tempP = pp->Proportion * (sin(1.0*abs(pp->PrevError)/6.28)+0.4);
// pp->SumError += pp->PrevError;//λ��ʽpid�ǶԻ��ֵĳ����ۼӣ�������ɻ��ֱ��ͣ���ϵͳ����
// dutychange = tempP*pp->PrevError+pp->Integral*pp->SumError+pp->Derivative*(pp->PrevError - pp->LastError);
// pp->LastError = pp->PrevError;
// return dutychange;
//}

////����ʽpid
//float PID_realize_v3(PID* pp, double PreVal){
//	float incrementSpeed;
//	float tempP;
//	pp->PrevError=pp->SetPoint - PreVal;
//	if(pp->PrevError < 3.5 && pp->PrevError > -3.5)
//		pp->PrevError = 0;
//	tempP = pp->Proportion * (sin(1.0*abs(pp->PrevError)/6.28)+0.4);
//	incrementSpeed=tempP*(pp->PrevError-pp->LastError)+pp->Integral*pp->PrevError+pp->Derivative*(pp->PrevError-2*pp->LastError+pp->LastError0);//ֻ��ǰ�����ε����ֵ�йأ�Ҳ�������
//	pp->LastError0 = pp->LastError;
//	pp->LastError = pp->PrevError;
//	return  incrementSpeed;
//}
