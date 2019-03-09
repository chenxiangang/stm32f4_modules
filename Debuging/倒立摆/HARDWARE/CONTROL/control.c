#include "control.h"
#include "delay.h"
#include "math.h"
#include "pid.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "SSI4096.h"

int PreError = 0; //�������
int LastError = 0; //�ϴ����
extern PID sPID;
extern PID pPID;
extern u16 pwmduty;
extern u16 Static_Pwmduty;
double cal_duty; //pid����õ���ֵ
double position_cal_duty;

extern int ActionFlag;
extern double Angle;
extern int speed;
extern u16 MaxPWM;


void Start_Swing()
{
    motor_action(400);  //˳ʱ��ת��
	  delay_ms(300);
	  motor_action(-200);
	  delay_ms(200);
	ActionFlag = 2;
}

void Revert()
{
	cal_duty = PIDCalc(&sPID, Angle);  //�Ա������ǶȽ���PID���㣬Ŀ��ֵΪ180     �����������и�
	position_cal_duty = position_PIDCalc(&pPID,speed);  //�Ե���ٶȽ���PID���㣬Ŀ��ֵΪ0    ������Ϊ��ֵ
	if(cal_duty > 0)
		motor_action(cal_duty + position_cal_duty);
	else 
		motor_action(cal_duty - position_cal_duty);
	
	
}

//������ʱ��
//����˳ʱ��
//720����
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

