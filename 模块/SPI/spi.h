#ifndef _SPI_H_
#define _SPI_H_
#include "sys.h"
//����ʹ��SPI2  ʹ�ø�ģ���Ӧ����NRF24L01

//RCCʱ�Ӻ궨��
#define SPI_MISO_RCC       RCC_AHB1Periph_GPIOB    //SPI MISOʹ�õ�GPIOʱ��  ����ӳ�
#define SPI_MOSI_RCC       RCC_AHB1Periph_GPIOB    //SPI MOSIʹ�õ�GPIOʱ��  ��������
#define SPI_SCLK_RCC			 RCC_AHB1Periph_GPIOB    //SPI SCLKʹ�õ�GPIOʱ��
#define SPI_RCC            RCC_APB1Periph_SPI2    //SPI���ص�ʱ��
//���ڲ�ͬSPI���ص�ʱ�Ӳ�һ�����Լǵú�������ģ�ʹ��SPI2����Ϊ�����϶�ӦSPI2

//SPI  GPIO���ź궨��
#define SPI_MISO_Pin  		GPIO_Pin_14     //PB14
#define SPI_MOSI_Pin			GPIO_Pin_15     //PB15
#define SPI_SCLK_Pin			GPIO_Pin_13			//PB13

//SPI ʹ�õ�GPIO
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

//APB1���42M�����ڶ�ʱ��84M����ͨ����42M
//���ܼǴ�
//�������� _ΪС���㣬ϣ�����˰��Ҹĸ�
//����SPI2   ������APB1��SPI
//SPI �ٶ�
#define SPI_21M        			  SPI_BaudRatePrescaler_2 
#define SPI_10_5M       			SPI_BaudRatePrescaler_4 
#define SPI_5_25M        			SPI_BaudRatePrescaler_8 
#define SPI_2_625M        		SPI_BaudRatePrescaler_16 
#define SPI_1_3125M        		SPI_BaudRatePrescaler_32 
#define SPI_656_25K       		SPI_BaudRatePrescaler_64 
#define SPI_328_125K        	SPI_BaudRatePrescaler_128
#define SPI_164_0625K        	SPI_BaudRatePrescaler_256 

void My_SPI_Init(void);  //��ʼ������    ֮ǰ������������
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SPI_BaudRatePrescaler);  //�����ٶȣ���һ����Ҫ���õ�SPI �ڶ������ٶ�������
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData);   //��д  ��һ�����������õ�SPI ���ڶ�����д�����ݣ����ض�������
void SPI_WriteByte(SPI_TypeDef* SPIx , u8 TxData);  //дһ�����ݣ��������溯�����
u8 SPI_ReadByte(SPI_TypeDef* SPIx );  //��һ������,��д�������
#endif
