/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 11:23:45
 * @LastEditTime: 2019-03-09 12:54:24
 */
/*
*把所有东西都放TIM2中断里面，注意中断时间不能太长，最好10ms以内
*pid.c里面有三个函数，分别是不同算法的，用了一个sin函数来解决大P小P的问题
*编码器倒立时和下垂时的编码值在SSI4096.c开头那里设置就行了
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

float Balance_KP = 7, Balance_KD = 20, Position_KP = 0.5, Position_KD = 50; // PID系数
int top_voltage = 1998, Position_Zero = 15000;								//电位器目标值  位置目标值

int speed;
u16 pwmduty;
u16 MaxPWM = 720;
u16 Static_Pwmduty;
u16 uccr1;
u16 nccr1;
PID sPID;
PID pPID;

extern int Error;
extern double cal_duty; //pid计算得到的值
extern double Angle;

int ActionFlag = 2; //1表示可以开始进行转圈，2表示可以开始进行倒立

int main(void)
{
	pwmduty = 0;
	cal_duty = 0;
	Static_Pwmduty = pwmduty;

	sPID.PrevError = 0;
	sPID.SumError = 0;
	sPID.LastError = 0;
	sPID.LastError0 = 0;
	sPID.SetPoint = 181.9; // 角度目标值

	pPID.PrevError = 0;
	pPID.SumError = 0;
	pPID.LastError = 0;
	pPID.LastError0 = 0;
	pPID.SetPoint = 0; // 角度速度值

	//位置式角度pid参数
	sPID.Proportion = 100;
	sPID.Integral = 0.04;
	sPID.Derivative = 170;

	//位置式位置pid参数
	pPID.Proportion = 0.0;
	pPID.Integral = 0.00;
	pPID.Derivative = 0.0;

	//增量式pid参数
	//		sPID.Proportion = 100;
	//		sPID.Integral = 0.0;
	//		sPID.Derivative = 6;

	delay_init(168);								//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);								//串口初始化为115200
	LED_Init();										// LED端口初始化
	TIM3_PWM_Init(1799, 3);						//84MHz  84000000/(2000)/(4)	//不分频。PWM频率=72000000/900/5=16Khz
	TIM2_Int_Init(20, 7199);					//84000000/(21)/8000
	TIM4_Encoder_Init();
	SSI_init();
	ActionFlag = 0; //起摆标志
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
