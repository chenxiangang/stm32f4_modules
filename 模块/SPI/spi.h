#ifndef _SPI_H_
#define _SPI_H_
#include "sys.h"
//本次使用SPI2  使用该模块的应该是NRF24L01

//RCC时钟宏定义
#define SPI_MISO_RCC       RCC_AHB1Periph_GPIOB    //SPI MISO使用的GPIO时钟  主入从出
#define SPI_MOSI_RCC       RCC_AHB1Periph_GPIOB    //SPI MOSI使用的GPIO时钟  主出从入
#define SPI_SCLK_RCC			 RCC_AHB1Periph_GPIOB    //SPI SCLK使用的GPIO时钟
#define SPI_RCC            RCC_APB1Periph_SPI2    //SPI挂载的时钟
//由于不同SPI挂载的时钟不一样所以记得函数里面改，使用SPI2是因为板子上对应SPI2

//SPI  GPIO引脚宏定义
#define SPI_MISO_Pin  		GPIO_Pin_14     //PB14
#define SPI_MOSI_Pin			GPIO_Pin_15     //PB15
#define SPI_SCLK_Pin			GPIO_Pin_13			//PB13

//SPI 使用的GPIO
#define SPI_MISO_GPIO     GPIOB
#define SPI_MOSI_GPIO     GPIOB
#define SPI_SCLK_GPIO     GPIOB

//SPI AF
#define SPI_AF   						GPIO_AF_SPI2    //SPI2
#define SPI_MISO_AF_PIN 		GPIO_PinSource14  
#define SPI_MOSI_AF_PIN 		GPIO_PinSource15
#define SPI_SCLK_AF_PIN			GPIO_PinSource13

//SPI
#define SPI_Use             SPI2

//APB1最高42M，用于定时器84M，普通外设42M
//可能记错
//命名鬼畜 _为小数点，希望有人帮我改改
//适用SPI2   挂载在APB1的SPI
//SPI 速度
#define SPI_21M        			  SPI_BaudRatePrescaler_2 
#define SPI_10_5M       			SPI_BaudRatePrescaler_4 
#define SPI_5_25M        			SPI_BaudRatePrescaler_8 
#define SPI_2_625M        		SPI_BaudRatePrescaler_16 
#define SPI_1_3125M        		SPI_BaudRatePrescaler_32 
#define SPI_656_25K       		SPI_BaudRatePrescaler_64 
#define SPI_328_125K        	SPI_BaudRatePrescaler_128
#define SPI_164_0625K        	SPI_BaudRatePrescaler_256 

void My_SPI_Init(void);  //初始化函数    之前的重名。。。
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SPI_BaudRatePrescaler);  //设置速度，第一个是要设置的SPI 第二个是速度在上面
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData);   //读写  第一个参数是设置的SPI ，第二个是写的数据，返回读的数据
void SPI_WriteByte(SPI_TypeDef* SPIx , u8 TxData);  //写一个数据，来自上面函数拆分
u8 SPI_ReadByte(SPI_TypeDef* SPIx );  //读一个数据,读写函数拆分
#endif
