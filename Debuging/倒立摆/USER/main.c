/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 11:23:45
 * @LastEditTime: 2019-03-09 12:54:24
 */
/*
*�����ж�������TIM2�ж����棬ע���ж�ʱ�䲻��̫�������10ms����
*pid.c�����������������ֱ��ǲ�ͬ�㷨�ģ�����һ��sin�����������PСP������
*����������ʱ���´�ʱ�ı���ֵ��SSI4096.c��ͷ�������þ�����
*/
//#include "FreeCars_uart.h"
#include "control.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "pid.h"
#include "speed_cap.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "SSI4096.h"

float Balance_KP = 7, Balance_KD = 20, Position_KP = 0.5, Position_KD = 50; // PIDϵ��
int top_voltage = 1998, Position_Zero = 15000;								//��λ��Ŀ��ֵ  λ��Ŀ��ֵ

int speed; 
u16 pwmduty;
u16 MaxPWM = 720;
u16 Static_Pwmduty;
u16 uccr1;
u16 nccr1;
PID sPID;
PID pPID;

extern int Error;
extern double cal_duty; //pid����õ���ֵ
extern double Angle;

int ActionFlag = 2; //1��ʾ���Կ�ʼ����תȦ��2��ʾ���Կ�ʼ���е���

int main(void)
{
	pwmduty = 0;
	cal_duty = 0;
	Static_Pwmduty = pwmduty;

	sPID.PrevError = 0;
	sPID.SumError = 0;
	sPID.LastError = 0;
	sPID.LastError0 = 0;
	sPID.SetPoint = 181.9; // �Ƕ�Ŀ��ֵ

	pPID.PrevError = 0;
	pPID.SumError = 0;
	pPID.LastError = 0;
	pPID.LastError0 = 0;
	pPID.SetPoint = 0; // �Ƕ��ٶ�ֵ

	//λ��ʽ�Ƕ�pid����
	sPID.Proportion = 100;
	sPID.Integral = 0.04;
	sPID.Derivative = 170;

	//λ��ʽλ��pid����
	pPID.Proportion = 0.0;
	pPID.Integral = 0.00;
	pPID.Derivative = 0.0;

	//����ʽpid����
	//		sPID.Proportion = 100;
	//		sPID.Integral = 0.0;
	//		sPID.Derivative = 6;

	delay_init(168);								//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);								//���ڳ�ʼ��Ϊ115200
	LED_Init();										// LED�˿ڳ�ʼ��
	TIM3_PWM_Init(1799, 3);						//84MHz  84000000/(2000)/(4)	//����Ƶ��PWMƵ��=72000000/900/5=16Khz
	TIM2_Int_Init(20, 7199);					//84000000/(21)/8000
	TIM4_Encoder_Init();
	SSI_init();
	ActionFlag = 0; //��ڱ�־
					//	motor_action(100);
					//	while(Angle < 10 && Angle > 350);
	while (1)
	{
		//			push(0,(u16)Angle*10);
		//			push(1,(u16)sPID.SetPoint*10);
		//			push(2,(u16)sPID.Proportion);
		//			push(3,(u16)sPID.Integral);
		//			push(4,(u16)sPID.Derivative);
		//			push(5,(u16)pPID.Proportion);
		//			push(6,(u16)pPID.Integral);
		//			push(7,(u16)pPID.Derivative);
		////			Angle_Cal();
		//      uSendOnePage();
	}
}
