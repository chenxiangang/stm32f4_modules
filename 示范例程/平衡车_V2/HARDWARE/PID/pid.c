#include "pid.h"
#include "math.h"

PID JYAngle_PID; //利用JY901读到的数据进行控制
PID Encoder_Angle_PID; //电机编码器测到的角度作为控制量
PID Speed_PID; //利用电机速度来控制
PID Turn_PID;  //转速环

//传入pid结构体和误差量，返回反馈量,即电机的pwm输出
void PIDCalc(PID* pp, double PreVal) // PreError为当前误差值
{
    double dutychange;
    double dError;
    double tempP;
    pp->PrevError = pp->SetPoint - PreVal;

    //在设定的允许误差范围内不再进行调节
    if (pp->PrevError < pp->allowError && pp->PrevError > -pp->allowError) {
        pp->PrevError = 0;
        pp->SumError = 0;
        pp->LastError = 0;
    }
    //可以分段进行P的设置
    tempP = pp->Proportion;

    //在一定范围内才开始积分
    if (pp->PrevError < pp->allowError * 6 && pp->PrevError > -pp->allowError * 6)
        pp->SumError = pp->SumError + pp->PrevError; // 积分，历史偏差累加
    else
        pp->SumError = 0;

    dError = pp->PrevError - pp->LastError; // 微分，偏差相减
    pp->LastError = pp->PrevError;

    dutychange = tempP * pp->PrevError + pp->Integral * pp->SumError + pp->Derivative * dError;

    //限幅
    // if (dutychange > 800)
    //     dutychange = 800;
    // else if (dutychange < -800)
    //     dutychange = -800;
    // else {
    // }
    pp->pwmduty = dutychange;
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
