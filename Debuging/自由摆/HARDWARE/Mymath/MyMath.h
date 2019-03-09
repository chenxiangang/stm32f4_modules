#ifndef __MYMATH_H
#define __MYMATH_H

#include "stdint.h"
#include "math.h"
//#include "common.h"

#define _PI	3.1415926535897932f
#define TAN_MAP_RES     0.003921569f     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293f
#define TAN_MAP_SIZE    256
#define MY_PPPIII   3.14159f
#define MY_PPPIII_HALF   1.570796f

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//卡尔曼滤波
void KalmanFilterInit(void);
float KalmanFilter(float Measure);

float my_abs(float f);
float fast_atan2(float y, float x);
float my_atan(float x, float y);
float my_asin(float r);
float my_acos(float r);
float my_pow(float a,int n);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float x,float zoom);
float my_deathzoom_2(float x,float zoom);
float To_180_degrees(float x);
float my_pow_2_curve(float in,float a,float max);
int my_4out5in(double x);

double superposition_Km(int n,double m);				//第一类完全椭圆积分
double integration(double a,double b,int n);			//一般定积分，函数自己改
double simple(double a,double omega,double t);			//简谐运动，输入量均为国际单位，弧度制
double T_calculate_big(double alpha,double length);		//大角度摆动周期计算，输入输出均为国际单位
float rectangle(float up,float down,float T_s,float pwm,float t_ms);


#endif
