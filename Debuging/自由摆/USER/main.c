#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "led.h"
#include "oled.h"
#include "pid.h"
#include "sys.h"
#include "timer.h" //ʵ�ֿ��ƹ��ܵĶ�ʱ��
#include "usart.h"

FLAG_Typedef flag;

PID pid1;
PID pid2;
int a;
double pwmduty;
int angle0;
extern double Angle;

int main(void)
{
    pid1.SumError = 0;
    pid1.PrevError = 0;
    pid1.LastError = 0;
    pid1.LastError0 = 0;
    pid1.SetPoint = 0;

    pid2.SumError = 0;
    pid2.PrevError = 0;
    pid2.LastError = 0;
    pid2.LastError0 = 0;
    pid2.SetPoint = -3;

    pid1.Proportion = 0;
    pid1.Integral = 0.00;
    pid1.Derivative = 0;

    pid2.Proportion = 50;
    pid2.Integral = 0.04;
    pid2.Derivative = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
    delay_init(168); //��ʼ����ʱ����
    LED_Init(); //��ʼ��LED�˿�
    uart_init(115200); //��ʼ������1������Ϊ115200
    usart3_init(115200); //������ȡ�����ǵ�����
    TIM2_Int_Init(20, 7199); //��ʼ�����ƶ�ʱ��
    Encoder_TIM4_Init();
    BTN7971_Init(1); //EN1��������
    OLED_Init();
    SSI_init();

    while (1) {
        Angle_Cal();
        //ǿ��ת����int���Ͳ�����freecars����ʾ����
        push(0, (int)roll * 10); //�����ǵ�ֵ
        push(1, (int)Angle * 10);
        push(2, (int)Read_Encoder()); //���ת���ĽǶ�
        push(3, (int)pwmduty); //pwm���
        push(4, (int)pid2.SetPoint * 10);
        uSendOnePage();
    }
}
