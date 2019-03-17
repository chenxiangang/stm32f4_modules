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
		encoder_angle = Read_Encoder();  //��ȡĿǰ�������Ƕ�
		
		if(pid1.SetPoint - encoder_angle> 180 ||pid1.SetPoint - encoder_angle < -180)    //��ԭ������Ȧ����ʱ��ȫ�ٷ�ת
		{
			if(encoder_angle > 180)
				contrl_speed(1080,1);
			else 
				contrl_speed(-1080,1);
		}
		
		else                                               //��ԭ������Ȧ����ʱ����ʼ���٣���Ϊ������Ҫ������Ҫ������
		{
			pwmduty = PIDCalc(&pid1,encoder_angle);         //����pid����
			
			if(pwmduty > 0)
				contrl_speed(pwmduty+80,1);                    //��2100��arr�£���80������
			else
				contrl_speed(pwmduty-80,1);
		}
}
 

void SetAngle(double Setangle)
{
	  pid1.SetPoint = Setangle;  
	  
		pwmduty = PIDCalc(&pid1,Read_Encoder());
		if(pwmduty > 0)
				contrl_speed(pwmduty+110,1);                     //��2100��arr�£���80������
			else
				contrl_speed(pwmduty-110,1);
				
}



