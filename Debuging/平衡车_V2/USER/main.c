#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "led.h"
#include "mynvic.h"
#include "pid.h"
#include "sys.h"
#include "tb6612.h"
#include "usart.h"
#include "usmart.h"

/*
���Թܣ� PD0 PD1 PD2 PD3 
ENCODER1��   PD12 PD13  (TIM4)
ENCODER2�� ����
TB6612��PWM��PA6 PA7(Ŀǰ��LED��ͻ)   (TIM3)          AIN1 AIN2 BIN1 BIN2   PA0 PA1 PA2 PA3    STBY�ӵ�Դ3.3V
JY901:     USART3  TX PD8    RX PD9     
OLED:   CS  PD3 
        RST PD4
				DC  PD5
				D0  PD6
				D1  PD7

*/

FLAG_Typedef flag;

void changePID(PID *pp,u8 p,u8 i,u8 d)
{
	pp->Proportion = p;
	pp->Integral = i;
	pp->Derivative = d;
}

void PID_Init()
{
    //�ǶȻ�������ʼ��
    JYAngle_PID.SetPoint = 0;
    JYAngle_PID.LastError0 = 0;
    JYAngle_PID.LastError = 0;
    JYAngle_PID.PrevError = 0;
    JYAngle_PID.SumError = 0;
    JYAngle_PID.pwmduty = 0;
    //�ٶȻ�������ʼ��
    Speed_PID.SetPoint = 0;
    Speed_PID.LastError0 = 0;
    Speed_PID.LastError = 0;
    Speed_PID.PrevError = 0;
    Speed_PID.SumError = 0;
    Speed_PID.pwmduty = 0;

    //�ǶȻ�PID
    JYAngle_PID.Proportion = 0;
    JYAngle_PID.Integral = 0;
    JYAngle_PID.Derivative = 0;

    //�ٶȻ�PID
    Speed_PID.Proportion = 0;
    Speed_PID.Integral = 0;
    Speed_PID.Derivative = 0;
}

int main(void)
{

    delay_init(168); //��ʼ����ʱ����
    LED_Init(); //��ʼ��LED�˿�
    PID_Init(); //PID��ʼ��
		usmart_dev.init(84); 	//��ʼ��USMART
    YL_70_Init(); //��ʼ�����Թ�
    Encoder_TIM2_Init(); //��ʼ�����������B
    Encoder_TIM4_Init(); //��ʼ�����������A
    TIM5_Init(100, 7199); //��ȡ���������ݣ�����pid����
    uart_init(115200); //��ʼ������1�����ڷ������ݵ���λ��
    usart3_init(115200); //������ȡ�����ǵ�����
    My_NVIC_Init(); //�����ж����ȼ�
    TB6612_Init(); //���������ʼ��

    while (1) {
			//speedcontrol(1500,1,500);
			//speedcontrol(1500,2,-500);
    }
}
