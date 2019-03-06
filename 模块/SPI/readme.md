<!--
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-06 20:43:27
 * @LastEditTime: 2019-03-06 21:13:58
 -->

## HOW TO USE

### 接法
- PB13  SCLk
- PB14  MISO
- PB15  MOSI

### 注意
注释乱码，晚点加回来
 ```
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_21M; //?????
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial=7;	
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b；
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex；
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;  
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; 
```

### 解释
- CPOL=0，表示当SCLK=0时处于空闲态，所以有效状态就是SCLK处于高电平时；
- CPOL=1，表示当SCLK=1时处于空闲态，所以有效状态就是SCLK处于低电平时；
- CPHA=0，表示数据采样是在第1个边沿，数据发送在第2个边沿；
- CPHA=1，表示数据采样是在第2个边沿，数据发送在第1个边沿。

- CPOL=0，CPHA=0：此时空闲态时，SCLK处于低电平，数据采样是在第1个边沿，也就是SCLK由低电平到高电平的跳变，所以数据采样是在上升沿；
- CPOL=0，CPHA=1：此时空闲态时，SCLK处于低电平，数据发送是在第1个边沿，也就是SCLK由低电平到高电平的跳变，所以数据采样是在下降沿；
- CPOL=1，CPHA=0：此时空闲态时，SCLK处于高电平，数据采集是在第1个边沿，也就是SCLK由高电平到低电平的跳变，所以数据采集是在下降沿；
- CPOL=1，CPHA=1：此时空闲态时，SCLK处于高电平，数据发送是在第1个边沿，也就是SCLK由高电平到低电平的跳变，所以数据采集是在上升沿。


![](spi.png)