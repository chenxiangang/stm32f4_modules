/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-15 19:46:16
 * @LastEditTime: 2019-03-16 14:24:59
 */
#include "CCD.h"
#include "delay.h"

int CCD_Value[128] = {0};     //CCD的值 不带DMA
u16 DMA_CCD_Value[128] = {0}; //CCD的值 使用DMA
int CCD_Yuzhi;                //CCD阈值
int CCD_Zhongzhi;             //CCD中线

//CCD的初始化
//没有DMA
void CCD_Init_ND(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;           //GPIO初始化
    ADC_CommonInitTypeDef ADC_CommonInitStructure; //ADC共同结构体
    ADC_InitTypeDef ADC_InitStructure;             //ADC初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //使能ADC77

    //初始化GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; //PC1 PC2
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);                 //初始化

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    //模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //PC3
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //浮空
    GPIO_Init(GPIOC, &GPIO_InitStructure);           //初始化

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //复位结束

    //规则通道？
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      //DMA使能
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //四分频  84MHz/4
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样周期5个时钟
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //软件触发
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      // 12bit
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //非扫描模式
    ADC_Init(ADC1, &ADC_InitStructure);                                         //ADC初始化

    //ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles); //规则1 480周期

    ADC_Cmd(ADC1, ENABLE); //开启AD转换器
}

//软件转换ADC的值
//没有DMA
u16 Get_Adc(u8 ch)
{
    //指定ADC采样通道，一个序列，采样时间
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles); //ADC1,ADCÍ¨µÀ,480¸öÖÜÆÚ,Ìá¸ß²ÉÑùÊ±¼ä¿ÉÒÔÌá¸ß¾«È·¶È

    ADC_SoftwareStartConv(ADC1); //启动软件转换

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ; //等待转换结束

    return ADC_GetConversionValue(ADC1); //返回转换值
}

//CCD的数据采集
void read_CCD_ND(void)
{
    int i; //循环变量

    //开始转换信号
    CCD_SI = 0;
    CCD_CLK = 1;
    delay_us(10); //我也不知道多少
    CCD_SI = 1;
    CCD_CLK = 0;
    delay_us(10);
    CCD_SI = 0;
    CCD_CLK = 1;
    delay_us(10);

    //取值
    for (i = 0; i < 128; i++)
    {
        CCD_CLK = 0;
        delay_us(10); //我也不知道多少
        CCD_Value[i] = Get_Adc(13);
        CCD_CLK = 1; //时钟信号
        delay_us(10);
    }
}

//找中线
//没有DMA
//简化版
void CCD_Find_Mid_Nd()
{
    static int i, j, Left, Right;
    for (i = 5; i < 118; i++) //左跳边沿
    {
        if (CCD_Value[i] > CCD_Yuzhi && CCD_Value[i + 1] > CCD_Yuzhi && CCD_Value[i + 2] > CCD_Yuzhi && CCD_Value[i + 3] < CCD_Yuzhi && CCD_Value[i + 4] < CCD_Yuzhi && CCD_Value[i + 5] < CCD_Yuzhi)
        {
            Left = i;
            break;
        }
    }
    for (j = 118; j > 5; j--) //右跳边沿
    {
        if (CCD_Value[j] < CCD_Yuzhi && CCD_Value[j + 1] < CCD_Yuzhi && CCD_Value[j + 2] < CCD_Yuzhi && CCD_Value[j + 3] > CCD_Yuzhi && CCD_Value[j + 4] > CCD_Yuzhi && CCD_Value[j + 5] > CCD_Yuzhi)
        {
            Right = j;
            break;
        }
    }
    CCD_Zhongzhi = (Right + Left) / 2; //中值
}

//CCD初始化
//使用DMA
//先不要用吧，感觉有点问题
void CCD_Init_DMA(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;           //GPIO初始化
    ADC_CommonInitTypeDef ADC_CommonInitStructure; //ADC共同结构体
    ADC_InitTypeDef ADC_InitStructure;             //ADC初始化结构体
    DMA_InitTypeDef DMA_InitStructure;             //DMA初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //使能ADC77
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  //DMA2使能

    //初始化GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; //PC1 PC2
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);                 //初始化

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    //模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //PC3
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //浮空
    GPIO_Init(GPIOC, &GPIO_InitStructure);           //初始化

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //复位结束

    //规则通道？
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      //DMA使能
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //四分频  84MHz/4
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样周期5个时钟
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //软件触发
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      // 12bit
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //非扫描模式
    ADC_Init(ADC1, &ADC_InitStructure);                                         //ADC初始化

    DMA_DeInit(DMA2_Stream0);
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
    {
    } //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);                //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_CCD_Value;                 //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //外设到存储器
    DMA_InitStructure.DMA_BufferSize = 128;                                     //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA_PeripheralDataSize_Byte;//外设数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //DMA_MemoryDataSize_Byte;//存储器数据长度:16
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 使用循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //高等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                      //不使用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;               //FIFO全传输
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         //外设突发单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);                                 //初始化DMA Stream

    //ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles); //规则1 480周期

    ADC_Cmd(ADC1, ENABLE);         //开启AD转换器
    DMA_Cmd(DMA2_Stream0, ENABLE); //使能DMA2 通道0
    //ADC_SoftwareStartConv(ADC1);                      //软件传输
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //ADC数据变化后开始转换
    ADC_DMACmd(ADC1, ENABLE);                         //使能ADC1的DMA模式
}

void DMA_Read_CCD()
{
    int i = 0;
    //开始转换信号
    CCD_SI = 0;
    CCD_CLK = 1;
    delay_us(10); //我也不知道多少
    CCD_SI = 1;
    CCD_CLK = 0;
    delay_us(10);
    CCD_SI = 0;
    CCD_CLK = 1;
    delay_us(10);

    for (i = 0; i < 128; i++)
    {
        CCD_CLK = 0;
        delay_us(10); //我也不知道多少
        //TODO  //使用单次转换
        ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles); //规则1 480周期
        ADC_SoftwareStartConv(ADC1);                                                 //软件传输
        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))                               //等待转换结束
            ;
        CCD_CLK = 1; //时钟信号
        delay_us(10);
    }
    // ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles); //规则1 480周期

    // ADC_SoftwareStartConv(ADC1);                                                 //软件转换开始
    // ADC_DMACmd(ADC1, ENABLE);                                                    //使能ADC1的DMA模式
    // while (DMA_GetCurrDataCounter(DMA2_Stream0))                                 //剩余传输个数
    // {
    //     if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF7) != RESET) //传输完成
    //     {
    //         DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF7); //清除传输位
    //     }
    // }
}

void DMA_CCD_Find_Mid()
{
    static int i, j, Left, Right;
    for (i = 5; i < 118; i++) //左跳边沿
    {
        if (DMA_CCD_Value[i] > CCD_Yuzhi && DMA_CCD_Value[i + 1] > CCD_Yuzhi && DMA_CCD_Value[i + 2] > CCD_Yuzhi && DMA_CCD_Value[i + 3] < CCD_Yuzhi && DMA_CCD_Value[i + 4] < CCD_Yuzhi && DMA_CCD_Value[i + 5] < CCD_Yuzhi)
        {
            Left = i;
            break;
        }
    }
    for (j = 118; j > 5; j--) //右跳边沿
    {
        if (DMA_CCD_Value[j] < CCD_Yuzhi && DMA_CCD_Value[j + 1] < CCD_Yuzhi && DMA_CCD_Value[j + 2] < CCD_Yuzhi && DMA_CCD_Value[j + 3] > CCD_Yuzhi && DMA_CCD_Value[j + 4] > CCD_Yuzhi && DMA_CCD_Value[j + 5] > CCD_Yuzhi)
        {
            Right = j;
            break;
        }
    }
    CCD_Zhongzhi = (Right + Left) / 2; //中值
}
