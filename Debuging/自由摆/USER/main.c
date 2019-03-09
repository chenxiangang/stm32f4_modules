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
#include "timer.h" //实现控制功能的定时器
#include "usart.h"

FLAG_Typedef flag;

PID pid1;
PID pid2;

extern double SSI_Angle; //16384编码器测得的角度

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

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
    delay_init(168); //初始化延时函数
    LED_Init(); //初始化LED端口
    uart_init(9600); //初始化串口1波特率为115200
    usart3_init(115200); //用来读取陀螺仪的数据
    //	TIM2_Int_Init(100, 7199);  //初始化控制定时器
    Encoder_TIM4_Init();
    BTN7971_Init(1); //EN1组电机工作
    OLED_Init();
    SSI_init();

    while (1) {
        Angle_Cal();
        printf("%lf\r\n", Read_Encoder());
        //强制转换成int类型才能在freecars上显示负数
        //		push(0,(int)roll*10);  //陀螺仪的值
        //		push(1,(int)Angle*10);
        ////		push(2,(int)swingflag);  //电机转过的角度
        //		push(3,(int)Read_Encoder()*10);  //pwm输出
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
        //				contrl_speed(pwmduty + 80,1);   //100为死区
        //			else if (pwmduty < 0)
        //				contrl_speed(pwmduty - 80,1);   //100为死区
        //       swing();
        swing();
    }
}
