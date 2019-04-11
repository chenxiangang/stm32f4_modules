/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-04-05 12:16:28
 * @LastEditTime: 2019-04-05 13:38:14
 */
#include "OV7620.h"
#include "OV7620_REG.h"
#include "dcmi.h"
#include "sccb.h"


u8 Image[CAMERA_H+5][CAMERA_W]= {0};

CamCtrlClass ImgControl;
/*OV7620初始化配置表*/
Register_Info ov7620_reg[] =
    {
#if 1 //QVGA \
    //寄存器，寄存器值次
        {CLKRC, 0x00},                       //分频
        {COMC, 0x24},                        //0x24 QVGA 0x04 VGA
        {HSTART, (0x83 - CAMERA_W / 2 / 2)}, //截取图像，图像校正   减小--左移，增大--右移
        {HSIZE, (0x83 + CAMERA_W / 2 / 2)},
        {VSTRT, (0X7a - CAMERA_H / 2)},
        {VSIZE, (0X7a + CAMERA_H / 2)},
        {VEEE, 0x42}, //垂直边缘增强
        {COMH, 0x00}, //扫描模式
#else                 //VGA
        //寄存器，寄存器值次
        {CLKRC, 0x00},
        //{COMC      ,0x04},  //0x24 QVGA 0x04 VGA
        {HSTART, (0x7F - CAMERA_W / 4 / 2)},
        {HSIZE, (0x7F + CAMERA_W / 4 / 2)},
        {VSTRT, (0X7E - CAMERA_H / 2)},
        {VSIZE, (0X7E + CAMERA_H / 2) - 1},
//{VEEE      ,0x42},  //垂直边缘增强
//{COMH      ,0x20},  //扫描模式
#endif

};
u8 cfgnum = sizeof(ov7620_reg) / sizeof(ov7620_reg[0]); /*结构体数组成员数目*/


/************************************************
 * 函数名：Ov7620_reg_Init
 * 描述  ：Sensor 寄存器 初始化
 * 输入  ：无
 * 输出  ：返回1成功，返回0失败
 * 注意  ：无
 ************************************************/
u8 Ov7620_reg_Init(void)
{
    u16 i = 0;
    //  参数说明： address---写数据的寄存器
    //            data---写的内容                返回1成功
    while (SCCB_WR_Reg(0x12, 0xA4)) /*复位sensor */
    {
        i++;
        if (i == 20)
        {
            return 0;
        }
    }
    for (i = 0; i < cfgnum; i++)
    {
        while (SCCB_WR_Reg(ov7620_reg[i].Address, ov7620_reg[i].Value))
        {
            //            Send_DataPlusStr("Warning：write register ",ov7620_reg[i].Address," error!\n" );
            return 0;
        }
    }
    //    Send_Str("OV7620 Register Config Success!\n");    //time_delay_ms(1000);
    return 1;
}

/**
 * @description: OV7620初始化
 * @param {type} void
 * @return:    1 成功
 *             0 失败 
 */
u8 OV7620_Init(void)
{
    SCCB_Init();    //初始化IIC
    My_DCMI_Init(); //DCMI初始化
    DCMI_DMA_Init((uint32_t)&(Image[0]),CAMERA_W,DMA_PeripheralDataSize_HalfWord,DMA_MemoryInc_Enable);

    if (!Ov7620_reg_Init())
    {
        return 0;
    }
    return 1;
}
