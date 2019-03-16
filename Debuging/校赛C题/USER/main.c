/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 14:29:02
 * @LastEditTime: 2019-03-16 14:34:42
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "mynvic.h"
#include "CAN.h"

#define top_psc (84 - 1)     //Ԥ��Ƶ��ֵ
#define top_arr (0xFFFFFFFF) //��װ�س�ֵ

//��λ������
//Ŀǰ����ת��
int main(void)
{
  int buffer_len = 0; //CAN��ȡ�ĳ���
  int t_mode = 1;     //ͨ�ŷ�ʽ PWM 0  ���� CAN 1 ��ʼCAN
  int out_mode;       //����ת�ٻ��Ƿ���
  u16 out_msg;        //�����Ϣ
  uart_init(115200);
  delay_init(168);
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  My_CAN_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN��ʼ��
  TIM2_CH4_Cap_Init(top_psc, top_arr);                                    //���벶���ʼ��  84MHz/84 1M��ʼ����
  //TO DO key��ʼ��
  //TO DO �����ĳ�ʼ��
  while (1)
  {
    if (t_mode) //CANģʽ
    {
      buffer_len = CAN_Receive_16(&out_mode, &out_msg); //��ȡ��Ϣ
      if (buffer_len)                                   //������
      {
        //TO DO �����źŴ���
      }
    }
    else
    {

      //TO DO
    }
  }
}
