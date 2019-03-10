#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "control.h"
#include "delay.h"
#include "encoder.h"
#include "include.h"
#include "mynvic.h" //�ж����ȼ�����
#include "oled.h"
#include "pid.h"
#include "sys.h"
#include "timer.h" //ʵ�ֿ��ƹ��ܵĶ�ʱ��
#include "usart.h"

FLAG_Typedef flag;

double MotoAngle;  //�������ⲿ����

PID pid1;
PID PID_Moto;

extern double SSI_Angle; //16384��������õĽǶ�

void Sys_Init(void) //��ʼ����ģ���ϵͳ����
{
  My_NVIC_Init();
  delay_init(168);   //��ʼ����ʱ����
  uart_init(115200); //��ʼ������1������Ϊ115200
  //usart3_init(115200); //������ȡ�����ǵ�����
  //TIM2_Int_Init(100, 7199); //��ʼ�����ƶ�ʱ��
  //��ʼ��˳�򽻻��ж���Ͳ����������
  Encoder_TIM4_Init();
  TIM2_Int_Init(100, 7199); //��ʼ�����ƶ�ʱ��
  BTN7971_Init(1);          //EN1��������
  OLED_Init();
  SSI_init();
}

void Pid_Init(void)
{
  pid1.SumError = 0;
  pid1.PrevError = 0;
  pid1.LastError = 0;
  pid1.LastError0 = 0;
  pid1.SetPoint = 0;

  PID_Moto.SumError = 0;
  PID_Moto.PrevError = 0;
  PID_Moto.LastError = 0;
  PID_Moto.LastError0 = 0;
  PID_Moto.SetPoint = 0;

  pid1.Proportion = 0;
  pid1.Integral = 0.00;
  pid1.Derivative = 0;

  PID_Moto.Proportion = 200;
  PID_Moto.Integral = 0.08;
  PID_Moto.Derivative = 10;
}

int main(void)
{
  //    double MotoAngle;
  Sys_Init();
  Pid_Init();
  while (1)
  {
    // ǿ��ת����int���Ͳ�����freecars����ʾ����
    //        push(1, (int)SSI_Angle * 10);
    //        push(2, (int)Read_Encoder() * 10); //pwm���
    //        uSendOnePage();
    //        MotoAngle = Read_Encoder();  //���������ֵ��while��������ʱ���Ӱ��
    //        push(0, (int)SSI_Angle * 10);
    //        push(1, (int)MotoAngle * 10);
    //        push(2, (int)(SSI_Angle + MotoAngle) * 100);
    //        uSendOnePage();
    //       swing();
    printf("%f\r\n", MotoAngle);
    //printf("SSI_Angle:%f  MotoAngle:%f  Balance:%f\r\n",SSI_Angle,MotoAngle,SSI_Angle+MotoAngle);
  }
}
