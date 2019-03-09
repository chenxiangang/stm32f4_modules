#include "dma.h"

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
	
} 

void dma_adc_init(u32 mar,u16 ndtr)
{
	GPIO_InitTypeDef GPIOinitstruct;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
	
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//等待DMA可配置 
	
	 /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  //通道选择
	//之间貌似是外设通道错了
  DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralToMemory;//外设到存储器
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord; //DMA_PeripheralDataSize_Byte;//外设数据长度:16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//DMA_MemoryDataSize_Byte;//存储器数据长度:16
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream	
		
	GPIOinitstruct.GPIO_Mode=GPIO_Mode_AN;     //PA5为ADC1通道
	GPIOinitstruct.GPIO_Pin=GPIO_Pin_5;
	GPIOinitstruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIOinitstruct);      
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMA失能
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;  //独立模式
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;  //四分频  84MHz/4
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;  //两个采样周期5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//关闭连续转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right ; //右对齐
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;   //软件触发
	ADC_InitStructure.ADC_NbrOfConversion=1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b; // 12bit
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;  //非扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
 
  DMA_Cmd(DMA2_Stream0,ENABLE);    //使能DMA2
    
  
  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_480Cycles);  //规则1 480周期
  
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_SoftwareStartConv(ADC1); 
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);    //ADC1开启DMA
	ADC_DMACmd(ADC1, ENABLE);    //使能ADC1的DMA模式
	//ADC_SoftwareStartConv(ADC1); 
}
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  
