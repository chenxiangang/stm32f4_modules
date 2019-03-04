#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

//ALIENTEK 探索者STM32F407开发板 实验13
//LCD显示实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main()
{	
	u8 i=0;
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度
	u8 res;
	
	delay_init(168);
	LED_Init();
	uart_init(115200);
	TFTLCD_Init();			//LCD初始化
	MPU6050_Init();					//初始化MPU6050
	
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
	FRONT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(10,200,tftlcd_data.width,tftlcd_data.height,16," Temp:    . C");	
 	LCD_ShowString(10,220,tftlcd_data.width,tftlcd_data.height,16,"Pitch:    . C");	
 	LCD_ShowString(10,240,tftlcd_data.width,tftlcd_data.height,16," Roll:    . C");	 
 	LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16," Yaw :    . C");	
	
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU6050_Get_Temperature();	//得到温度值
			MPU6050_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU6050_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			
			if((i%10)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(10+48,200,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(10+48,200,' ',16,0);		//去掉负号 
				printf("检测温度为:%d度\r\n",temp);
				LCD_ShowNum(10+48+8,200,temp/100,3,16);		//显示整数部分	    
				LCD_ShowNum(10+48+40,200,temp%10,1,16);		//显示小数部分 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,220,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(10+48,220,' ',16,0);		//去掉负号 
				printf("Pitch:%d\r\n",temp);
				LCD_ShowNum(10+48+8,220,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(10+48+40,220,temp%10,1,16);		//显示小数部分 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,240,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(10+48,240,' ',16,0);		//去掉负号 
				printf("Roll:%d\r\n",temp);
				LCD_ShowNum(10+48+8,240,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(10+48+40,240,temp%10,1,16);		//显示小数部分 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(10+48,260,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(10+48,260,' ',16,0);		//去掉负号 
				printf("Yaw:%d\r\n",temp);
				LCD_ShowNum(10+48+8,260,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(10+48+40,260,temp%10,1,16);		//显示小数部分  
				LED1=!LED1;
			}
		}		
		i++;			
	}
}
