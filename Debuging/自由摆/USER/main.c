#include "FreeCars_uart.h"
#include "JY901_uart.h"
#include "SSI4096.h"
#include "btn7971.h"
#include "control.h"
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

extern double SSI_Angle; //16384��������õĽǶ�

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
    pid2.SetPoint = 0;

    pid1.Proportion = 0;
    pid1.Integral = 0.00;
    pid1.Derivative = 0;

    pid2.Proportion = 200;
    pid2.Integral = 0.08;
    pid2.Derivative = 10;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
    delay_init(168); //��ʼ����ʱ����
    LED_Init(); //��ʼ��LED�˿�
    uart_init(9600); //��ʼ������1������Ϊ115200
    usart3_init(115200); //������ȡ�����ǵ�����
    //	TIM2_Int_Init(100, 7199);  //��ʼ�����ƶ�ʱ��
    Encoder_TIM4_Init();
    BTN7971_Init(1); //EN1��������
    OLED_Init();
    SSI_init();

    while (1) {
        Angle_Cal();
        printf("%lf\r\n", Read_Encoder());
        //ǿ��ת����int���Ͳ�����freecars����ʾ����
        //		push(0,(int)roll*10);  //�����ǵ�ֵ
        //		push(1,(int)Angle*10);
        ////		push(2,(int)swingflag);  //���ת���ĽǶ�
        //		push(3,(int)Read_Encoder()*10);  //pwm���
        //		push(4,(int)-pid2.SetPoint*10);
        //		uSendOnePage();
        //
        //		get_SSI();
        //		printf("%lf\r\n",Angle);

        //		if(swingflag == 0)
        //		{
        //			pid2.SetPoint = -Angle * 0.42 - 6;
        //			if(Angle >20 && Angle <= 30)
        //				pid2.SetPoint = -Angle * 0.65 - 6;
        //			else if(Angle >10 && Angle <= 20)
        //				pid2.SetPoint = -Angle * 0.4 - 6;
        //			else if(Angle >0 && Angle <= 10)
        //				pid2.SetPoint = -Angle * 0.4 - 6;
        //			else if(Angle < 0)
        //				swingflag = 1;
        //			else
        //			{}
        //		}
        //
        //		if(swingflag == 1)
        //		{
        //			if(pid2.SetPoint < 20 || pid2.SetPoint > -20)
        //				pid2.SetPoint = -Angle * 0.6 -6;
        //			else if(pid2.SetPoint >20  &&pid2.SetPoint <40  || pid2.SetPoint <-20 && pid2.SetPoint > -40)
        //				pid2.SetPoint = -Angle * 0.7 -6;
        //			else
        //				pid2.SetPoint = -Angle * 0.75 -6;
        //		}
        //
        //
        //
        //
        //
        //
        //
        //			pwmduty = PIDCalc(&pid2,-roll);
        //
        //		  if(pwmduty > 0)
        //				contrl_speed(pwmduty + 80,1);   //100Ϊ����
        //			else if (pwmduty < 0)
        //				contrl_speed(pwmduty - 80,1);   //100Ϊ����
        //       swing();
        swing();
    }
}
