/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 14:29:02
 * @LastEditTime: 2019-03-18 20:58:29
 */
 
//0  ת��
//1  �Ƕ�
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "mynvic.h"
#include "CAN.h"
#include "timercap.h"
#include "led.h"
#include "btn7971.h"
#include "encoder.h"
#include "oled.h"
#include "pid.h"
#include "control.h"
#include "key.h"

#define top_psc (84 - 1)     //���벶��Ԥ��Ƶ��ֵ
#define top_arr (0xFFFFFFFF) //���벶����װ�س�ֵ

int t_mode = 1; //ͨ�ŷ�ʽ PWM 0  ���� CAN 1 ��ʼCAN
u8 out_mode;    //����ת�ٻ��Ƿ���
u16 out_msg;    //�����Ϣ

PID pid1;
PID pid2;
extern double pwmduty;
//��λ������
//Ŀǰ����ת��
int main(void)
{
  //int buffer_len = 0; //CAN��ȡ�ĳ���

  pid1.SumError = 0; //pid������ʼ��
  pid1.PrevError = 0;
  pid1.LastError = 0;
  pid1.LastError0 = 0;
  pid1.SetPoint = 0;

  pid1.Proportion = 3;
  pid1.Integral = 0.0;
  pid1.Derivative = 5;
	
	pid2.SumError = 0; //pid������ʼ��
  pid2.PrevError = 0;
  pid2.LastError = 0;
  pid2.LastError0 = 0;
  pid2.SetPoint = 0;

  pid2.Proportion = 20;
  pid2.Integral = 0.0;
  pid2.Derivative = 0;
	
	
  uart_init(115200);
  delay_init(168);
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                       //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  BTN7971_Init(1);                                                          //���������ʼ��
  Encoder_TIM4_Init();                                                      //��������ʼ��
  OLED_Init();                                                              //oled��ʼ��
  LED_Init();                                                               //LED��ʼ��
  My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN��ʼ��  CAN_Mode_Normal ����ģʽ
  TIM2_CH4_Cap_Init(top_psc, top_arr);                                      //���벶���ʼ��  84MHz/84 1M��ʼ����
  KEY_Init();                                                               //������ʼ��
	TIM3_Int_Init(1000-1,84-1);//1ms
  My_NVIC_Init();                                                           //�ж�����  �о��������ȼ�Ҫע��һ��
  while (1)
  {
    LED1 = !LED1;                                            //��ʾ����������
    OLED_ShowMPU(Read_Encoder(), pwmduty, pid1.Integral, 0); //��ʾ��ֵ
    if (KEY_Scan(0))                                         //��֧�������� KEY0
    {
      t_mode = !t_mode;            //ģʽת��
      LED0 = !LED0;                //��ʾģʽ
      ClearAll();
			out_mode = 0;
      out_msg = 0;
    }
    // if (t_mode) //CANģʽ
    // {
    //   buffer_len = CAN_Receive_16(&out_mode, &out_msg); //��ȡ��Ϣ
    //   if (buffer_len)                                   //������
    //   {
    //     //TO DO ��Ҫ��λ��
    //     if (out_mode) //ת�Ƕ�
    //     {
    //       //TO DO �����źŴ���
    //     }
    //     else //תȦ��
    //     {
    //     }
    //   }
    // }
    // else //PWM����ģʽ
    // {
    //   PWM_STA = PWM_Get_msg(&out_mode, &out_msg); //�����ź�
    //   if (PWM_STA)                                //������
    //   {
    //     //TO DO ��Ҫ��λ��
    //     if (out_mode) //ת�Ƕ�
    //     {
    //       //TO DO �����źŴ���
    //     }
    //     else //תȦ��
    //     {
    //     }
    //   }
    // }
    // //TO DO ����ԭ״
  }
}
