/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-11 20:17:30
 * @LastEditTime: 2019-03-14 16:56:20
 */

#include "CAN.h"

//未完成
//先用CAN1吧，正点原子的板子上有转接芯片
//中断之后加
//tsjw:重新同步跳跃时间单元 范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp 波特率分频器1~1024；tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,正常模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1初始化42M，如果设置My_CAn_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//波特率42M/((6+7+1)*6)=500kbps
void My_CAN_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;           //GPIO结构体
    CAN_InitTypeDef CAN_InitStructure;             //CAN初始化结构体
    CAN_FilterInitTypeDef CAN_FilterInitStructure; //CAN滤波器初始化结构体，滤波器是找ID用的
#ifndef _MYNVIC_H_
    NVIC_InitTypeDef NVIC_InitStructure; //NVIC初始化结构体
#endif

    //开启相关的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  //CAN1

    //GPIO初始化
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             //复用
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; //PA11 RX     PA12 TX
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   //初始化PA11 PA12

    //复用引脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //A11 复用CAN1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //A12 复用CAN1

    //CAN结构体初始化
    //这个结构体还是没搞懂，嘤嘤嘤
    //CAN_StructInit(&CAN_InitStructure;  也许可以用这个自动初始化，具体结果没试过
    CAN_InitStructure.CAN_ABOM = DISABLE;  //软件自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;  //睡眠模式，通过软件唤醒
    CAN_InitStructure.CAN_BS1 = tbs1;      //Tbs1范围 CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2 = tbs2;      //Tbs2范围 CAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Mode = mode;     //模式设置 共4种模式
    CAN_InitStructure.CAN_NART = ENABLE;   //禁止报文自动传送
    CAN_InitStructure.CAN_Prescaler = brp; //分频系数 Fdiv=brp+1
    CAN_InitStructure.CAN_RFLM = DISABLE;  //报文不锁定，旧的覆盖新的
    CAN_InitStructure.CAN_SJW = tsjw;      //重新同步跳跃时间宽度(tsjw)为 tswj+1 个时间单位   1~4
    CAN_InitStructure.CAN_TTCM = DISABLE;  //非时间触发通道模式
    CAN_InitStructure.CAN_TXFP = DISABLE;  //报文优先级由报文标识符决定
    CAN_Init(CAN1, &CAN_InitStructure);    //初始化CAN1

    //滤波器结构体的初始化
    //具体结构体还不是很懂
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               //激活过滤器
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                   ////32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;                    //
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;               //32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;                //
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;      //
    CAN_FilterInitStructure.CAN_FilterNumber = 0;                        //过滤器0
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;     //32位
    CAN_FilterInit(&CAN_FilterInitStructure);                            //初始化过滤器

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //挂号中断使能允许

#ifndef _MYNVIC_H_
    //中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;       //接收中断
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;                //主优先级1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //次优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ENABLE;   //使能
#endif
}
//中断服务函数
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    int i = 0;
    CAN_Receive(CAN1, 0, &RxMessage);
    for (i = 0; i < 8; i++)
        printf("rxbuf[%d]:%d\r\n", i, RxMessage.Data[i]);
}

//can：发送一组数据（固定格式ID为0x12，标准帧，数据帧）
//len:数据长度（最大为8）
//msg:数据指针（最大为8字节）
//返回：0，成功
//	   其他，失败
u8 CAN1_Send_Msg(u8 *msg, u8 len)
{
    u8 mbox;
    u16 i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = 0x12; // 标准标识符0
    TxMessage.ExtId = 0x12; //设置拓展标识符（29位）
    TxMessage.IDE = 0;      // 使用拓展标识符
    TxMessage.RTR = 0;      //消息类型为数据帧（8位）
    TxMessage.DLC = len;
    for (i = 0; i < len; i++)
        TxMessage.Data[i] = msg[i];
    mbox = CAN_Transmit(CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
        i++; //等待发送结束
    if (i >= 0XFFF)
        return 1;
    return 0;
}
//can：接收数据查询
//buf：数据缓存区
//返回值:0,无数据被接收；
//		 其他，数据接收的长度;
u8 CAN1_Receive_Msg(u8 *buf)
{
    u32 i;
    CanRxMsg RxMessage;
    if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
        return 0;                             //没有接收到数据，直接退出
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //读取数据
    for (i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i];
    return RxMessage.DLC;
}

//发送PWM信号
//返回0 成功
//   其他 失败
//mode 模式
//0 转速
//1 角度
//msg 16位的信息
u8 CAN_SEND_CONTORL(u8 mode, u16 msg)
{
    u8 buffer[3];       //3位是因为只需要3位，按要求改
    buffer[0] = mode;         //模式
    buffer[1] = msg >> 8;     //高8位
    buffer[2] = msg & 0x00FF; //低8位
    return (CAN1_Send_Msg(buffer, 3));
}

//接收16位
//msg 16位的数据
//返回 0 没读到
//    其他 读到
u8 CAN_Receive_16(u8* mode,u16 *msg)
{
    u8 len;
    u8 buffer[8];
    len = CAN1_Receive_Msg(buffer);
    if (len) //读到了
    {
        *mode = buffer[0];                    //模式赋值
        *msg = (buffer[1] << 8) | buffer[2]; //合成
    }
    return len;
}