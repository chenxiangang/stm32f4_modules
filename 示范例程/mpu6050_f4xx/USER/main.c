#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

//ALIENTEK ̽����STM32F407������ ʵ��13
//LCD��ʾʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main()
{	
	u8 i=0;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�
	u8 res;
	
	delay_init(168);
	LED_Init();
	uart_init(115200);
	TFTLCD_Init();			//LCD��ʼ��
	MPU6050_Init();					//��ʼ��MPU6050
	
	FRONT_COLOR=BLACK;
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"MPU6050 Test");
	
	FRONT_COLOR=RED;
	res=mpu_dmp_init();
	printf("res=%d\r\n",res);
	while(mpu_dmp_init())
	{
		printf("MPU6050 Error!\r\n");
		LCD_ShowString(10,130,tftlcd_data.width,tftlcd_data.height,16,"MPU6050 Error!");
		delay_ms(200);
	}
	printf("MPU6050 OK!\r\n");
	LCD_ShowString(10,130,tftlcd_data.width,tftlcd_data.height,16,"MPU6050 OK!");
	FRONT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(10,200,tftlcd_data.width,tftlcd_data.height,16," Temp:    . C");	
 	LCD_ShowString(10,220,tftlcd_data.width,tftlcd_data.height,16,"Pitch:    . C");	
 	LCD_ShowString(10,240,tftlcd_data.width,tftlcd_data.height,16," Roll:    . C");	 
 	LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16," Yaw :    . C");	
	
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU6050_Get_Temperature();	//�õ��¶�ֵ
			MPU6050_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU6050_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			
			if((i%10)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(10+48,200,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(10+48,200,' ',16,0);		//ȥ������ 
				printf("����¶�Ϊ:%d��\r\n",temp);
				LCD_ShowNum(10+48+8,200,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(10+48+40,200,temp%10,1,16);		//��ʾС������ 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,220,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(10+48,220,' ',16,0);		//ȥ������ 
				printf("Pitch:%d\r\n",temp);
				LCD_ShowNum(10+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(10+48+40,220,temp%10,1,16);		//��ʾС������ 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(10+48,240,' ',16,0);		//ȥ������ 
				printf("Roll:%d\r\n",temp);
				LCD_ShowNum(10+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(10+48+40,240,temp%10,1,16);		//��ʾС������ 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,260,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(10+48,260,' ',16,0);		//ȥ������ 
				printf("Yaw:%d\r\n",temp);
				LCD_ShowNum(10+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(10+48+40,260,temp%10,1,16);		//��ʾС������  
				LED1=!LED1;
			}
		}		
		i++;			
	}
}
