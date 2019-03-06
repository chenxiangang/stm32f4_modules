#include "spi.h"

void My_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SPI_MISO_RCC|SPI_MOSI_RCC|SPI_SCLK_RCC, ENABLE);//使能GPIOB时钟
  RCC_APB1PeriphClockCmd(SPI_RCC, ENABLE);//使能SPI2时钟
	
	//GPIO配置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//100MHz
	
	//初始化结构体
	//MISO
	GPIO_InitStructure.GPIO_Pin=SPI_MISO_Pin;
	GPIO_Init(SPI_MISO_GPIO,&GPIO_InitStructure); 
	//MOSI
	GPIO_InitStructure.GPIO_Pin=SPI_MOSI_Pin;
	GPIO_Init(SPI_MOSI_GPIO,&GPIO_InitStructure);
	//SCLK
	GPIO_InitStructure.GPIO_Pin=SPI_SCLK_Pin; 
	GPIO_Init(SPI_SCLK_GPIO,&GPIO_InitStructure);
	
	//开启复用
	GPIO_PinAFConfig(SPI_MISO_GPIO,SPI_MISO_AF_PIN,SPI_AF);  //MISO复用
	GPIO_PinAFConfig(SPI_MOSI_GPIO,SPI_MOSI_AF_PIN,SPI_AF);  //MOSI复用
	GPIO_PinAFConfig(SPI_SCLK_GPIO,SPI_SCLK_AF_PIN,SPI_AF);  //SCLK复用
	
	
	//这里只针对SPI口初始化
	RCC_APB1PeriphResetCmd(SPI_RCC,ENABLE);//复位SPI2
	RCC_APB1PeriphResetCmd(SPI_RCC,DISABLE);//停止复位SPI2
	
	//预分频值
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_21M;
	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样     另一个参数是第一个跳变沿数据采集
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;	
	//串行同步时钟的空闲状态为高电平   高电平为空闲状态
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	//CRC值计算的多项式 提高通信可靠性，大于1即可
	SPI_InitStructure.SPI_CRCPolynomial=7;	
	//数据长8位     8位和16位可选
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b; 
	//双向全双工模式  可选参数还有半双工等
	SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex; 
	//第一位为数据最高位
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	//主机模式        可选参数还有从机
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;  
	//NSS信号软件触发 这是片选信号
	//硬件还有点不懂
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; 
	
	//初始化
	SPI_Init(SPI_Use, &SPI_InitStructure);
	
	SPI_Cmd(SPI_Use, ENABLE); //使能SPI外设
	
	SPI_ReadWriteByte(SPI_Use,0xff);//大家都写不知道为什么	 
}

//SPI 速度设置
//第一个参数设置的SPI
//第二个参数SPI速度
void SPI_SetSpeed(SPI_TypeDef* SPIx , u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPIx->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPIx->CR1|=SPI_BaudRatePrescaler;	//设置SPI速度 
	SPI_Cmd(SPIx,ENABLE); //使能SPI
} 

//改自正点原子
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据	
 		    
}

//写一个数据
void SPI_WriteByte(SPI_TypeDef* SPIx , u8 TxData)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个byte  数据
}

//读一个数据
u8 SPI_ReadByte(SPI_TypeDef* SPIx )
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据
}