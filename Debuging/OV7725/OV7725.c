/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 20:11:53
 * @LastEditTime: 2019-04-05 22:09:37
 */
#include "OV7725.h"
#include "ov7725config.h"
#include "dcmi.h"
#include "sccb.h"
#include "delay.h"

/**
 * @description: 找不到就自己先定义了
 * @param {type} 
 * @return: 
 */
#define OV7725_EAGLE_W 80 //定义摄像头图像宽度
#define OV7725_EAGLE_H 60 //定义摄像头图像高度

u8 Image[CAMERA_H][CAMERA_W] = {0};

CamCtrlClass ImgControl;

/*OV7725初始化配置表*/
reg_7725 ov7725_eagle_reg[] =
    {
        /*输出窗口设置*/
        {CLKRC, 0x00},  //clock config
        {COM7, 0x06},   //VGA RGB565
        {HSTART, 0x3f}, //水平起始位置
        {HSIZE, 0x50},  //水平尺寸
        {VSTRT, 0x03},  //垂直起始位置
        {VSIZE, 0x78},  //垂直尺寸
        {HREF, 0x00},
        {HOutSize, 0x50}, //输出尺寸
        {VOutSize, 0x78}, //输出尺寸

        /*DSP control*/
        {TGT_B, 0x7f},
        {FixGain, 0x09},
        {AWB_Ctrl0, 0xe0},
        {DSP_Ctrl1, 0xff},
        {DSP_Ctrl2, 0x00},
        {DSP_Ctrl3, 0x00},
        {DSP_Ctrl4, 0x00},

        /*AGC AEC AWB*/
        {COM8, 0xf0},
        {COM4, 0x81}, /*Pll AEC CONFIG*/
        {COM6, 0xc5},
        {COM9, 0x11},
        {BDBase, 0x7F},
        {BDMStep, 0x03},
        {AEW, 0x40},
        {AEB, 0x30},
        {VPT, 0xa1},
        {EXHCL, 0x9e},
        {AWBCtrl3, 0xaa},
        {COM8, 0xff},

        /*matrix shapness brightness contrast*/
        {EDGE1, 0x08},
        {DNSOff, 0x01},
        {EDGE2, 0x03},
        {EDGE3, 0x00},
        {MTX1, 0xb0},
        {MTX2, 0x9d},
        {MTX3, 0x13},
        {MTX4, 0x16},
        {MTX5, 0x7b},
        {MTX6, 0x91},
        {MTX_Ctrl, 0x1e},
        {BRIGHT, 0x08},
        {CNST, 0x20},
        {UVADJ0, 0x81},
        {SDE, 0X06},
        {USAT, 0x65},
        {VSAT, 0x65},
        {HUECOS, 0X80},
        {HUESIN, 0X80},

        /*GAMMA config*/
        {GAM1, 0x0c},
        {GAM2, 0x16},
        {GAM3, 0x2a},
        {GAM4, 0x4e},
        {GAM5, 0x61},
        {GAM6, 0x6f},
        {GAM7, 0x7b},
        {GAM8, 0x86},
        {GAM9, 0x8e},
        {GAM10, 0x97},
        {GAM11, 0xa4},
        {GAM12, 0xaf},
        {GAM13, 0xc5},
        {GAM14, 0xd7},
        {GAM15, 0xe8},
        {SLOP, 0x20},

        {COM3, 0x50}, /*Horizontal mirror image*/
                      //注：datasheet默认0x10,即改变YUV为UVY格式。但是摄像头不是芯片而是模组时，
                      //要将0X10中的1变成0，即设置YUV格式。
        /*night mode auto frame rate control*/
        {COM5, 0xf5}, /*在夜视环境下，自动降低帧率，保证低照度画面质量*/

};

uint8_t ov7725_eagle_cfgnum = sizeof(ov7725_eagle_reg) / sizeof(ov7725_eagle_reg[0]); /*结构体数组成员数目*/

/**
 * @description: 对于OV7725寄存器的初始化
 * @param {type} 
 * @return: 1，成功
 *          0，失败
 */
uint8_t OV7725_reg_Init(void)
{
        int i;
        if (SCCB_WR_Reg(COM7, 0x80)) /*复位sensor */
        {
                //      DEBUG_PRINTF("\n警告:SCCB写数据错误");
                return 0;
        }
        delay_ms(50);
        for (i = 0; i < ov7725_eagle_cfgnum; i++)
        {
                if (SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val))
                {
                        //             DEBUG_PRINTF("\n警告:写寄存器0x%x失败", ov7725_eagle_reg[i].addr);
                        return 0;
                }
        }
        return 1;
}

/**
 * @description: 初始化OV7725
 * @param {type} 
 * @return:  1，成功
 *           0，失败
 */
u8 OV7725_Init(void)
{
        SCCB_Init();
        My_DCMI_Init();
        DCMI_DMA_Init((uint32_t) & (Image[0]), CAMERA_W, DMA_PeripheralDataSize_HalfWord, DMA_MemoryInc_Enable);
        if (!OV7725_reg_Init())
                return 0;
        return 1;
}

