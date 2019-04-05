/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 17:34:31
 * @LastEditTime: 2019-04-05 21:55:57
 */
#include "sys.h"
#include "dcmi.h"
#include "OV7725.h"

u8 frame = 0; //帧率
//extern void jpeg_data_process(void);	//JPEG数据处理函数

DCMI_InitTypeDef DCMI_InitStructure;

//DCMI中断服务函数
//中断中完成处理
//这个中断目前还没有处理函数
//记得测试一下这个中断大概要多少时间
void DCMI_IRQHandler(void)
{
	if (DCMI->MISR & DCMI_IT_VSYNC) //VSYNC capture
	{
		DCMI->ICR = DCMI_IT_VSYNC; //clear flag

		//		//图像采集完成，更换缓冲区
		//		if(Image_raw == ImageBuff1)
		//		{
		//			Image_raw = ImageBuff2;
		//		}
		//		if(Image_raw == ImageBuff2)
		//		{
		//			Image_raw = ImageBuff1;
		//		}
		//
		//		//如果图像处理完成，更换到图像采集的另一个缓冲区
		//		if(ImgControl.ImgDealOk == true)
		//		{
		//			if(Image_raw == ImageBuff1)
		//			{
		//				Image = ImageBuff2;
		//			}
		//			if(Image_raw == ImageBuff2)
		//			{
		//				Image = ImageBuff1;
		//			}
		//
		//			ImgControl.ImgDealOk = false;
		//		}

		ImgControl.lines = 0;

		//图像处理完成
		if (ImgControl.ImgDealOk)
		{
			ImgControl.lines = 0; //清空行
			ImgControl.ImgOk = false;

			//ImgControl.Count++;
			//64ms一次
		}
		frame++;
		//64ms一次
	}
	//1ms一次  0.5
	if (DCMI->MISR & DCMI_IT_LINE) //行结束
	{
		DCMI->ICR = DCMI_IT_LINE;

		if (ImgControl.ImgOk == false)
		{
			//1ms一次  0.5
			if (ImgControl.lines < CAMERA_H)
			{
				//1ms一次  0.5
				DMA2_Stream1->CR &= (uint32_t)~DMA_SxCR_EN;					 //DMA采集图像；每改变一次地址是需要将DMA失能后才能改变
				DMA2_Stream1->M0AR = (uint32_t) & (Image[ImgControl.lines]); //新的一行的地址
				DMA2_Stream1->CR |= DMA_SxCR_EN;							 //DMA2_Stream1 enable
				DCMI->CR |= DCMI_CR_CAPTURE;								 //capture enable
				ImgControl.lines++;											 //说明是一行一行扫描的
			}
			else
			{
				//                //未进入
				//                //RED_TURN;
				ImgControl.ImgOk = true;
				ImgControl.ImgDealOk = true;

				//                //	taskrun_time.camera.task_time=sys_time.s*1000+sys_time.ms-taskrun_time.camera.s*1000+taskrun_time.camera.ms;
				//                //DMA2_Stream1->CR  &= (uint32_t)~DMA_SxCR_EN;//DMA_Cmd(DMA2_Stream1, DISABLE);
			}
		}
	}

	if (DCMI->MISR & DCMI_IT_FRAME)
	{
		//未进入
		DCMI->ICR = DCMI_IT_FRAME;
	}
	if (DCMI->MISR & DCMI_IT_ERR)
	{
		//未进入
		DCMI->ICR = DCMI_IT_ERR;
	}
}
//DCMI使用DMA2 通道1的数据流1或者通道1的数据流7
//这里直接使用DMA2通道1的数据流1
//DCMI DMA配置
//DMA_Memory0BaseAddr:存储器地址    将要存储摄像头数据的内存地址(也可以是外设地址)
//DMA_BufferSize:存储器长度    0~65535
//DMA_MemoryDataSize:存储器位宽
//DMA_MemoryDataSize:存储器位宽    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:存储器增长方式  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr, u16 DMA_BufferSize, u32 DMA_MemoryDataSize, u32 DMA_MemoryInc)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
	{
	} //等待DMA2_Stream1可配置

	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;							//通道1 DCMI通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;				//外设地址为:DCMI->DR
	DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;			//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;						//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据长度:32位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;				//存储器数据长度，省赛模板是半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							// 使用循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//省赛模板里不使用FIFO模式不是很理解	//FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//使用全FIFO
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//外设突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//存储器突发单次传输
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);								//初始化DMA Stream

	//注意这里少了一个DMA_CMD函数
	/*****************************
	 * FIFO模式每次数据凑成一个字（16字节）后发送
	******************************/
}
//DCMI初始化
//修改完成
void My_DCMI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOA B  E 时钟
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);												//使能DCMI时钟
	//PA4/6初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10; //PA4/6/9/10   复用功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									  //复用功能输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;								  //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									  //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);											  //初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9; // PB6/7/8/9   复用功能输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);											 //初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4; //PE0/1/4  复用功能输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);								//初始化

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_DCMI);  //PA9,AF13  DCMI_D0
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_DCMI); //PA10,AF13 DCMI_D1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);  //PE0,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);  //PE1,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);  //PE4,AF13  DCMI_D4
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);  //PB6,AF13  DCMI_D5
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI);  //PB8,AF13  DCMI_D6
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);  //PB9,AF13  DCMI_D7

	DCMI_DeInit(); //清除原来的设置

	//看实际情况决定
	//看完OV7620后记得检查
	//按照省赛模板改完
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //连续模式
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;	//全帧捕获
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //8位数据格式
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;			 //水平同步下沿	 //HSYNC 低电平有效
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;		 //PCLK 上升沿有效
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;	 //硬件同步HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;			 //帧同步下沿	 //VSYNC 低电平有效
	DCMI_Init(&DCMI_InitStructure);

	//DCMI_ITConfig(DCMI_IT_FRAME, ENABLE); //开启帧中断
	DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE); //开启垂直中断
	DCMI_ITConfig(DCMI_IT_LINE, ENABLE);  //开启列中断

	DCMI_Cmd(ENABLE);		 //DCMI使能
	DCMI_CaptureCmd(ENABLE); //捕捉使能 还没搞懂是什么意思

	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器
}
//DMA自动转换还是手动转换有待商榷

#if 0
////////////////////////////////////////////////////////////////////////////////
//以下两个函数,供usmart调用,用于调试代码

//DCMI设置显示窗口
//sx,sy;LCD的起始坐标
//width,height:LCD显示范围.
void DCMI_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	DCMI_Stop();
	LCD_Clear(WHITE);
	LCD_Set_Window(sx, sy, width, height);
	OV2640_OutSize_Set(width, height);
	LCD_SetCursor(0, 0);
	LCD_WriteRAM_Prepare(); //开始写入GRAM

	DMA_Cmd(DMA2_Stream1, ENABLE); //开启DMA2,Stream1

	DCMI_CaptureCmd(ENABLE); //DCMI捕获使能
}

//通过usmart调试,辅助测试用.
//pclk/hsync/vsync:三个信号的有限电平设置
void DCMI_CR_Set(u8 pclk, u8 hsync, u8 vsync)
{
	DCMI_DeInit(); //清除原来的设置

	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //连续模式
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;	//全帧捕获
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //8位数据格式
	DCMI_InitStructure.DCMI_HSPolarity = hsync << 6;					 //HSYNC 低电平有效
	DCMI_InitStructure.DCMI_PCKPolarity = pclk << 5;					 //PCLK 上升沿有效
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;	 //硬件同步HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity = vsync << 7;					 //VSYNC 低电平有效
	DCMI_Init(&DCMI_InitStructure);

	DCMI_CaptureCmd(ENABLE); //DCMI捕获使能
	DCMI_Cmd(ENABLE);		 //DCMI使能
}
#endif
