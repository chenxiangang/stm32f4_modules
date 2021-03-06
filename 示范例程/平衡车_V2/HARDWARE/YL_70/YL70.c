/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 18:40:34
 * @LastEditTime: 2019-03-16 19:28:27
 */
#include "YL70.h"

u8 YL70[4];

/**
 * @description: 光电对管初始化  PD0~PD3
 * @param {type} void
 * @return: void
 * 遇到黑线灯灭，输出1
 */
void YL_70_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO初始化

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //开启GPIOD的时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                                     //输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                   //推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //PD0 PD1 PD2 PD3
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                                 //浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                               //100MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);                                           //初始化
}

/**
 * @description: 读单个光电对管的值
 * @param {type} ch 光电对管的通道
 * @return: 高低电平
 */
u8 YL_70_Read_One(u8 ch)
{
    return YL_70_IN(ch);
}

/**
 * @description: 读一组光电对管
 * @param {type} group 4位长的数组
 * @return: void
 */
void YL_70_Read_All(u8 *group)
{
    int i = 0;
    for (i = 7; i < 11; i++)
        group[i-7] = YL_70_Read_One(i); //读一组光电对管
}
