/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-15 19:46:24
 * @LastEditTime: 2019-03-15 21:53:12
 */
#ifndef _CCD_H_
#define _CCD_H_
#include "sys.h"

#define CCD_CLK PCout(1) //时钟输入 PC1
#define CCD_SI PCout(2)  //开始信号 PC2


void CCD_Init();  //CCD初始化
#endif
