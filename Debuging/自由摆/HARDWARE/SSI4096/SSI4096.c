#include "stm32f4xx.h"
#include "delay.h"
#include "SSI4096.h"
/******************************
*������16384�߾��Ա�����
*Ӳ���汾��Dragon2016
*�޸�ʱ�䣺2017/1/9
*����������ֵ��������ʼ��
*��Դ��code by dragon
*�޸��ˣ�hao
*******************************/

//�Ƕȱ��������
float CodeToA = 360.0 / 16384;  //������ת��Ϊ�Ƕ�
u16 bottom = 9804;
u16 top = 1612;

double Angle;
u16 SSI_LOC=0 ;//ȫ�ֱ���������λ��

void SSI_init()//IOģ���ʼ��
{
  GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//������ƬѡCS ��ʱ��CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;        		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     			
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;    		//��ʼ����������ʱ��ͼ��  		  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//��������DAT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void get_SSI()
{
  u8 i = 0;//����
  u8 crc = 0;
  SSI_LOC = 0;
	
	SSI_CLK = 0;
	SSI_CSN = 1;
	delay_us(5);
	
  SSI_CSN = 0;    //�����������
	SSI_CLK = 0;
  delay_us(5);
  SSI_CLK = 1;    //��һλ������λ���Ĵ���
	delay_us(5);
	SSI_CLK = 0;   //��ʼ�������,һ����������һλ
	delay_us(5);
	SSI_CLK = 1;
	delay_us(5);
	SSI_CLK = 0;  //�Ƶڶ�λ
	delay_us(5);  //�ڶ�λ����
	
	
  
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

