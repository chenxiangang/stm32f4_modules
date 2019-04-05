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
#define OV7725_EAGLE_W            80                                    //定义摄像头图像宽度
#define OV7725_EAGLE_H            60                                    //定义摄像头图像高度


u8 Image[CAMERA_H][CAMERA_W] = {0};

CamCtrlClass ImgControl;

/*OV7725初始化配置表*/
reg_7725 ov7725_eagle_reg[] =
    {
        //寄存器，寄存器值次
        {COM4, 0xC1},
        {CLKRC, 0x00},
        {COM2, 0x03},
        {COM3, 0xD0},
        {COM7, 0x40},
        {HSTART, 0x3F},
        {HSIZE, 0x50},
        {VSTRT, 0x03},
        {VSIZE, 0x78},
        {HREF, 0x00},
        {SCAL0, 0x0A},
        {AWB_Ctrl0, 0xE0},
        {DSPAuto, 0xff},
        {DSP_Ctrl2, 0x0C},
        {DSP_Ctrl3, 0x00},
        {DSP_Ctrl4, 0x00},

#if (OV7725_EAGLE_W == 80)
        {HOutSize, 0x14},
#elif (OV7725_EAGLE_W == 160)
        {HOutSize, 0x28},
#elif (OV7725_EAGLE_W == 240)
        {HOutSize, 0x3c},
#elif (OV7725_EAGLE_W == 320)
        {HOutSize, 0x50},
#else

#endif

#if (OV7725_EAGLE_H == 60)
        {VOutSize, 0x1E},
#elif (OV7725_EAGLE_H == 120)
        {VOutSize, 0x3c},
#elif (OV7725_EAGLE_H == 180)
        {VOutSize, 0x5a},
#elif (OV7725_EAGLE_H == 240)
        {VOutSize, 0x78},
#else

#endif

        {EXHCH, 0x00},
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
        {LC_RADI, 0x00},
        {LC_COEF, 0x13},
        {LC_XC, 0x08},
        {LC_COEFB, 0x14},
        {LC_COEFR, 0x17},
        {LC_CTR, 0x05},
        {BDBase, 0x99},
        {BDMStep, 0x03},
        {SDE, 0x04},
        {BRIGHT, 0x00},
        {CNST, 0xFF},
        {SIGN, 0x06},
        {UVADJ0, 0x11},
        {UVADJ1, 0x02},

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
