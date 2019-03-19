#include "pid.h"
#include "math.h"

//传入pid结构体和误差量，返回反馈量,即电机的pwm输出
double PIDCalc(PID* pp, double PreVal) // PreError为当前误差值
{
    double dutychange;
    double dError;
	  double tempP;
    double tempI;
    pp->PrevError = pp->SetPoint - PreVal;
	if(pp->PrevError < 1.0 && pp->PrevError > -1.0)
	{
		pp->PrevError = 0;
		pp->SumError = 0;
		pp->LastError = 0;
	}
	
		tempP = pp->Proportion;
		
//		if(pp->PrevError < 1.5 && pp->PrevError > -1.5)
//			tempP = 0;
//		else if((pp->PrevError >=  2 && pp->PrevError <  50) || (pp->PrevError >  -2 && pp->PrevError <=  -0.5))
//			tempP = tempP*0.8;
//		else
//			tempP = tempP*1.0;
	
	
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
		if(pp->PrevError < 6 && pp->PrevError > -6)
			 pp->SumError = pp->SumError + pp->PrevError; // 积分，历史偏差累加
		else
			pp->SumError = 0;

   
    dError = pp->PrevError - pp->LastError; // 微分，偏差相减,哪个减哪个?
    pp->LastError = pp->PrevError;

    dutychange = tempP * pp->PrevError + pp->Integral * pp->SumError + pp->Derivative * dError;

    if (dutychange > 800)
			 dutychange = 800;
		else if(dutychange < -800)
			dutychange = -800;
		else 
		{
		
		}
    return (dutychange);
}

//double position_PIDCalc(PID* pp, double PreVal) // PreError为当前误差值  位置PID
//{
//    double dutychange;
//    double dError;
//	  double tempP;
//    
//    pp->PrevError = PreVal - pp->SetPoint;

//	  
//    pp->SumError = pp->SumError + pp->PrevError; // 积分，历史偏差累加
//    dError = pp->PrevError - pp->LastError; // 微分，偏差相减,哪个减哪个?
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
// pp->SumError += pp->PrevError;//位置式pid是对积分的持续累加，容易造成积分饱和，是系统过调
// dutychange = tempP*pp->PrevError+pp->Integral*pp->SumError+pp->Derivative*(pp->PrevError - pp->LastError);
// pp->LastError = pp->PrevError;
// return dutychange;
//}

////增量式pid
//float PID_realize_v3(PID* pp, double PreVal){
//	float incrementSpeed;
//	float tempP;
//	pp->PrevError=pp->SetPoint - PreVal;
//	if(pp->PrevError < 3.5 && pp->PrevError > -3.5)
//		pp->PrevError = 0;
//	tempP = pp->Proportion * (sin(1.0*abs(pp->PrevError)/6.28)+0.4);
//	incrementSpeed=tempP*(pp->PrevError-pp->LastError)+pp->Integral*pp->PrevError+pp->Derivative*(pp->PrevError-2*pp->LastError+pp->LastError0);//只和前后三次的误差值有关，也方便计算
//	pp->LastError0 = pp->LastError;
//	pp->LastError = pp->PrevError;
//	return  incrementSpeed;
//}


