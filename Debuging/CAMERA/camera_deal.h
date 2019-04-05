#ifndef __CAMERA__DEAL__H__
#define __CAMERA__DEAL__H__

#include "sys.h"
#include "OV7620.h"

static int erasetable[256]=
{
    0,0,1,1,0,0,1,1,
    1,1,0,1,1,1,0,1,
    1,1,0,0,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,1,1,0,0,1,1,
    1,1,0,1,1,1,0,1,
    1,1,0,0,1,1,1,1,
    0,0,0,0,0,0,0,1,
    1,1,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    1,1,0,0,1,1,0,0,
    1,1,0,1,1,1,0,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,0,0,1,1,
    1,1,0,1,1,1,0,1,
    1,1,0,0,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,1,1,0,0,1,1,
    1,1,0,1,1,1,0,1,
    1,1,0,0,1,1,1,1,
    0,0,0,0,0,0,0,0,
    1,1,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,
    1,1,0,0,1,1,1,1,
    0,0,0,0,0,0,0,0,
    1,1,0,0,1,1,0,0,
    1,1,0,1,1,1,0,0,
    1,1,0,0,1,1,1,0,
    1,1,0,0,1,0,0,0
};


typedef struct
{
	uint32_t last_;			         /*上次阈值*/
	uint32_t new_;		           /*本次阈值*/
	uint32_t real;	
  uint32_t	foreground;                  /*前景均值*/
  uint32_t	background;                 /*背景均值*/
  uint32_t	count_front;                /*前景计数*/ 
  uint32_t	count_back;                   /*背景计数*/
	
	float offset;//阈值修正系数
}THRESHOLD;

extern u8 biImage[CAMERA_H][CAMERA_W];
extern THRESHOLD  threshold;
extern int16_t Threshold[10];


void Threshold_Camera(u8 limit);
uint8_t CalculateThreshold(uint8_t* src, uint16_t widthBottom, uint16_t heightBottom, uint16_t widthTop, uint16_t heightTop);
void Image2Bimap(u8 blackboard);
void Image_Bi2Bone(u8 bi_image[CAMERA_H][CAMERA_W]);
void core_find_average(u8* core_x,u8* core_y,u8 h_start,u8 w_start,u8 height,u8 weith,u8 blackboard,u8 thr);
void core_find_border(u8* core_x,u8* core_y,u8 h_start,u8 w_start,u8 height,u8 weith,u8 blackboard,u8 thr);



#endif


