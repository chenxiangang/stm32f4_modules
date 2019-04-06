#include "pid.h"
#include "math.h"

//����pid�ṹ�������������ط�����,�������pwm���
double PIDCalc(PID* pp, double PreVal) // PreErrorΪ��ǰ���ֵ
{
    double dutychange;
    double dError;
	  double tempP;
    double tempI;
    pp->PrevError = pp->SetPoint - PreVal;
	if(pp->PrevError < 0.5 && pp->PrevError > -0.5)
		pp->PrevError = 0;
	
		tempP = pp->Proportion;
		
		if(pp->PrevError < 1 && pp->PrevError > -1)
			tempP = 0;
		else if((pp->PrevError >=  2 && pp->PrevError <  50) || (pp->PrevError >  -2 && pp->PrevError <=  -0.5))
			tempP = tempP*0.8;
		else
			tempP = tempP*1.0;
//		
//		else if((pp->PrevError >=  9 && pp->PrevError <  14) || (pp->PrevError >  -14 && pp->PrevError <=  -9))
//			tempP = tempP*1.5;
//		
//		else if((pp->PrevError >=  14 && pp->PrevError < 18) || (pp->PrevError > -18 && pp->PrevError <=  -14))
//			tempP = tempP*2.6;
//		
//		else if((pp->PrevError >= 18 && pp->PrevError < 23) || (pp->PrevError > -23 && pp->PrevError <= -18))
//			tempP = tempP*3.0;
//		
//		else if((pp->PrevError >= 23 && pp->PrevError < 35) || (pp->PrevError > -35 && pp->PrevError <= -23))
//			tempP = tempP*3.4;
//	
		if(pp->PrevError < 10 && pp->PrevError > -10)
			tempI = pp->Integral;
		else
			tempI = 0;

    pp->SumError = pp->SumError + pp->PrevError; // ���֣���ʷƫ���ۼ�
    dError = pp->PrevError - pp->LastError; // ΢�֣�ƫ�����,�ĸ����ĸ�?
    pp->LastError = pp->PrevError;

    dutychange = tempP * pp->PrevError + tempI * pp->SumError + pp->Derivative * dError;

    if (dutychange > 800)
			 dutychange = 800;
		else if(dutychange < -800)
			dutychange = -800;
		else 
		{
		
		}
    return (dutychange);
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


