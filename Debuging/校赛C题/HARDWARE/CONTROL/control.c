#include "math.h"
#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "btn7971.h"
#include "oled.h"

double encoder_angle;
double pwmduty;
extern PID pid1;

void motorback()
{
		encoder_angle = Read_Encoder();  //读取目前编码器角度
		
		if(pid1.SetPoint - encoder_angle> 180 ||pid1.SetPoint - encoder_angle < -180)    //与原点相差半圈以上时，全速反转
		{
			if(encoder_angle > 180)
				contrl_speed(1080,1);
			else 
				contrl_speed(-1080,1);
		}
		
		else                                               //与原点相差半圈以内时，开始减速，因为精度有要求，所以要防超调
		{
			pwmduty = PIDCalc(&pid1,encoder_angle);         //进行pid运算
			
			if(pwmduty > 0)
				contrl_speed(pwmduty+80,1);                    //在2100的arr下，有80的死区
			else
				contrl_speed(pwmduty-80,1);
		}
}
 

void SetAngle(double Setangle)
{
	  pid1.SetPoint = Setangle;  
	  
		pwmduty = PIDCalc(&pid1,Read_Encoder());
		if(pwmduty > 0)
				contrl_speed(pwmduty+110,1);                     //在2100的arr下，有80的死区
			else
				contrl_speed(pwmduty-110,1);
				
}



