#include "control.h"
#include "delay.h"
#include "math.h"
#include "pid.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "SSI4096.h"

int PreError = 0; //本次误差
int LastError = 0; //上次误差
extern PID sPID;
extern PID pPID;
extern u16 pwmduty;
extern u16 Static_Pwmduty;
double cal_duty; //pid计算得到的值
double position_cal_duty;

extern int ActionFlag;
extern double Angle;
extern int speed;
extern u16 MaxPWM;


void Start_Swing()
{
    motor_action(400);  //顺时针转动
	  delay_ms(300);
	  motor_action(-200);
	  delay_ms(200);
	ActionFlag = 2;
}

void Revert()
{
	cal_duty = PIDCalc(&sPID, Angle);  //对编码器角度进行PID运算，目标值为180     计算结果有正有负
	position_cal_duty = position_PIDCalc(&pPID,speed);  //对电机速度进行PID运算，目标值为0    计算结果为正值
	if(cal_duty > 0)
		motor_action(cal_duty + position_cal_duty);
	else 
		motor_action(cal_duty - position_cal_duty);
	
	
}

//正数逆时针
//负数顺时针
//720满速
void motor_action(int speed)
{
	if(speed < 0)
	{
		speed = -speed;
		IN3 = 1;
		IN4 = 0;
	}
	else if(speed == 0)
	{
		IN3 = 0;
		IN4 = 0;
	}
	else if(speed > 0)
	{
		IN3 = 0;
		IN4 = 1;
	}
	if(speed > MaxPWM)
		speed = MaxPWM;
	speed = MaxPWM - speed;
	TIM_SetCompare2(TIM3, (u16)speed);
}

