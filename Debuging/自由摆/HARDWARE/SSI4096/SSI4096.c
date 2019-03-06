#include "stm32f4xx.h"
#include "delay.h"
#include "SSI4096.h"
/******************************
*描述：16384线绝对编码器
*硬件版本：Dragon2016
*修改时间：2017/1/9
*描述：绝对值编码器初始化
*来源：code by dragon
*修改人：hao
*******************************/

//角度编码器相关
float CodeToA = 360.0 / 16384;  //将编码转换为角度
u16 bottom = 9804;
u16 top = 1612;

double Angle;
u16 SSI_LOC=0 ;//全局变量编码器位置

void SSI_init()//IO模拟初始化
{
  GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//首先是片选CS 与时钟CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;        		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     			
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;    		//初始下拉，根据时序图来  		  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//数据引脚DAT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void get_SSI()
{
  u8 i = 0;//计数
  u8 crc = 0;
  SSI_LOC = 0;
	
	SSI_CLK = 0;
	SSI_CSN = 1;
	delay_us(5);
	
  SSI_CSN = 0;    //数据输出激活
	SSI_CLK = 0;
  delay_us(5);
  SSI_CLK = 1;    //第一位数据移位至寄存器
	delay_us(5);
	SSI_CLK = 0;   //开始数据输出,一次上升沿移一位
	delay_us(5);
	SSI_CLK = 1;
	delay_us(5);
	SSI_CLK = 0;  //移第二位
	delay_us(5);  //第二位舍弃
	
	
  
  for ( i = 0;i < 14;i++ )
  {
		SSI_CLK = 1;
	  delay_us(5);
    SSI_CLK = 0;
    SSI_LOC <<= 1;
    
    if ( SSI_DO )
    {
      crc = !crc;
      SSI_LOC |=1;
    }   
  }
	SSI_CLK = 0;
  SSI_CSN = 1;  
}

void Angle_Cal()
{
	int i;
	u32 SSI_LOC_Temp = 0;
	Angle = 0;
	for(i = 0; i < 10; i++)
	{
	get_SSI();
	SSI_LOC_Temp += SSI_LOC;
  }
	SSI_LOC = SSI_LOC_Temp / 10;
	Angle = 1.0 * SSI_LOC * CodeToA;
	Angle -= 1.0 * bottom * CodeToA;
	Angle = -Angle;
}

