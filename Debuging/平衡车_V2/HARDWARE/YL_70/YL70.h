/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 18:40:44
 * @LastEditTime: 2019-03-16 19:29:23
 */
#ifndef _YL70_H_
#define _YL70_H_
#include "sys.h"

//四个输入
/*
#define YL_70_IN1 PDin(0)
#define YL_70_IN2 PDin(1)
#define YL_70_IN3 PDin(2)
#define YL_70_IN4 PDin(3)

*/

extern u8 YL70[4]; //光电对管四个数据线的值，从左往右
//四个输入 n不要超过3
#define YL_70_IN(n) PDin(n) //第一次试这种骚操作

void YL_70_Init(void); //初始化
u8 YL_70_Read_One(u8 ch); //读单个光电对管的值
void YL_70_Read_All(u8* group); //读一组光电对管的值
#endif
