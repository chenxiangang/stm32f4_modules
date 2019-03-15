/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-15 19:46:16
 * @LastEditTime: 2019-03-15 21:52:54
 */
#include "CCD.h"

void CCD_Init()
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

    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //关闭连续转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //软件触发
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                      // 12bit
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                //非扫描模式
    ADC_Init(ADC1, &ADC_InitStructure);                                         //ADC初始化

    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles); //规则1 480周期

    ADC_Cmd(ADC1, ENABLE); //开启AD转换器
}
