/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-15 19:46:24
 * @LastEditTime: 2019-03-16 14:24:05
 */
#ifndef _CCD_H_
#define _CCD_H_
#include "sys.h"

#define CCD_CLK PCout(1) //时钟输入 PC1
#define CCD_SI PCout(2) //开始信号 PC2

extern int CCD_Value[128]; //CCD的值 不带DMA
extern u16 DMA_CCD_Value[128]; //CCD的值 使用DMA
extern int CCD_Yuzhi; //CCD阈值
extern int CCD_Zhongzhi; //CCD中线

void CCD_Init_ND(void); //CCD初始化没有DMA
u16 Get_Adc(u8 ch); //ADC的值 无DMA
void read_CCD_ND(void); //CCD的数据采集
void CCD_Find_Mid_Nd(void); //找中线 无CCD
void CCD_Init_DMA(void); //DMA_CCD初始化
void DMA_Read_CCD(void); //DMA_CCD读值
void DMA_CCD_Find_Mid(void); //DMA_CCD找中线
#endif
