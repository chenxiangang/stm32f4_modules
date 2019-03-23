#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "YL70.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "led.h"
#include "mynvic.h"
#include "oled.h"
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
int sendflag = 0; //������Ϣ��־
//-----------------usmart������------------
void JY_changePID(u8 p, u8 i, u8 d)
{
    JYAngle_PID.Proportion = p;
    JYAngle_PID.Integral = 1.0 * i/1000;
    JYAngle_PID.Derivative = d;
}
void Speed_changePID(int p, int i, int d)
{
    Speed_PID.Proportion = 1.0*p/100;
    Speed_PID.Integral = 1.0*i/100;
    Speed_PID.Derivative = d;
}
void send_info()
{
    sendflag = !sendflag;
}

void change_balancePoint(u8 flag,u8 val10)
{
	if(flag == 0)
	balance_point = - val10/10;
else
balance_point = val10/10;
JYAngle_PID.SetPoint = balance_point;
}
//------------------------------------
void PID_Init()
{
    //�ǶȻ�������ʼ��
    JYAngle_PID.SetPoint = balance_point;
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

    taskMode = keep_balance;
    delay_init(168); //��ʼ����ʱ����
    My_NVIC_Init(); //�����ж����ȼ�
    LED_Init(); //��ʼ��LED�˿�
    PID_Init(); //PID��ʼ��
    usmart_dev.init(84); //��ʼ��USMART
    YL_70_Init(); //��ʼ�����Թ�
    Encoder_TIM2_Init(); //��ʼ�����������B
    Encoder_TIM4_Init(); //��ʼ�����������A
    TIM5_Init(50 - 1, 8400 - 1); //��ȡ���������ݣ�����pid����
    uart_init(115200); //��ʼ������1�����ڷ������ݵ���λ��
    usart3_init(115200); //������ȡ�����ǵ�����
    TB6612_Init(); //���������ʼ��
    OLED_Init(); //OLED��ʼ��
    JY_changePID(70,5,15);//JY_changePID(100, 10, 17);  //����JY_changePID(70,10,17)
    Speed_changePID(100, 2, 0);
		change_balancePoint(0,75);//change_balancePoint(0,69);
    while (1) {
        //OLED_ShowMPU(JYAngle_PID.pwmduty,roll,pitch,yaw);
        //        push(0, (int)pitch);
        //        push(1, (int)JYAngle_PID.pwmduty);
        //        uSendOnePage();
        if (sendflag) {
            printf("pitch:%f	gyro:%f	pwmduty:%f\r\nspeed_PID.pwmduty:%f\r\n", pitch, gryo.y, pwmduty, Speed_PID.pwmduty);
						printf("%lf	%f\r\n",Left_Encoder_Angle,Right_Encoder_Angle);
            //printf("Encoder:%f  %f\r\n", Read_Encoder_L(), Read_Encoder_R());
            delay_ms(200);
        }
    }
}
