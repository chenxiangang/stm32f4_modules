#include "control.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "encoder.h"
#include "math.h"
#include "oled.h"
#include "pid.h"

double pwmduty = 200;
int swingflag;
extern PID PID_Moto;
extern double SSI_Angle;

/*****自由摆控制函数*******/
void swing(void)
{
    Angle_Cal();
    //	if(swingflag == 0)      //第一次下放时，摆杆角度大于30°，平板角度需约为22°
    //		{
    //			PID_Moto.SetPoint = -Angle * 0.42 - 6;
    //			if(Angle >20 && Angle <= 30)
    //				PID_Moto.SetPoint = -Angle * 0.65 - 6;
    //			else if(Angle >10 && Angle <= 20)
    //				PID_Moto.SetPoint = -Angle * 0.4 - 6;
    //			else if(Angle >0 && Angle <= 10)
    //				PID_Moto.SetPoint = -Angle * 0.4 - 6;
    //			else if(Angle < 0)
    //				swingflag = 1;
    //			else
    //			{}
    //		}
    //
    //		if(swingflag == 1)     //自由摆动
    //		{
    //			if(Angle < 20 || Angle > -20)
    //				PID_Moto.SetPoint = -Angle * 0.6 -6;
    //			else if(Angle >20  && Angle <40  || Angle <-20 && Angle > -40)
    //				PID_Moto.SetPoint = -Angle * 0.7 -6;
    //			else
    //				PID_Moto.SetPoint = -Angle * 0.75 -6;
    //
    PID_Moto.SetPoint = -SSI_Angle;
    pwmduty += PID_realize_v3(&PID_Moto, Read_Encoder());

    if (pwmduty > 0)
        contrl_speed(pwmduty + 80, 1); //100为死区
    else if (pwmduty < 0)
        contrl_speed(pwmduty - 80, 1); //100为死区
    //		}
}

/****激光笔控制函数*****/
void laser_pointer(void)
{
    Angle_Cal();
    //	if(Angle < 0)
    //		PID_Moto.SetPoint = -atan((1-cos(-Angle*0.01745)) / (1.6667-sin(-Angle*0.01745)))*180/3.141593 -12 ;
    //	else
    //		PID_Moto.SetPoint = -atan((1-cos(Angle*0.01745)) / (1.6667+sin(Angle*0.01745)))*180/3.141593 -12 ;

    PID_Moto.SetPoint = -6;
    //	pwmduty = PIDCalc(&PID_Moto,-roll);
    pwmduty = PIDCalc(&PID_Moto, -SSI_Angle - Read_Encoder());

    if (pwmduty > 0)
        contrl_speed(pwmduty + 80, 1); //100为死区
    else if (pwmduty < 0)
        contrl_speed(pwmduty - 80, 1); //100为死区
}
