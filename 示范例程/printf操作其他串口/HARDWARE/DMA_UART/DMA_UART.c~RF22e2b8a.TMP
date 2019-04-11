#include "DMA_UART.h"

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量
void MYDMA_Config(DMA_Stream_TypeDef* DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
    } else {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE) {
    } //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = chx; //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = par; //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mar; //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = ndtr; //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA_Streamx, &DMA_InitStructure); //初始化DMA Stream
}

void uart2_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    //	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOD时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2); //GPIOD5复用为USART2 TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); //GPIOD6复用为USART2 RX

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOD5与GPIOD6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化PD5，PD6

    //USART2 初始化设置
    USART_InitStructure.USART_BaudRate = bound; //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口2

    USART_Cmd(USART2, ENABLE); //使能串口2

    USART_ClearFlag(USART2, USART_FLAG_TC);
}

/* void SendOV7620(void)
{
    uint32_t i, j;
    uint8_t d;
    USendOneChar(0x01);
    USendOneChar(0xFE);
    for (i = 0; i < CAMERA_H; i++)
    {
        for (j = 0; j < CAMERA_W; j++)
        {
            d = Image[i][j];
            //if(d > 0xFD) d = 0xFD;
            USendOneChar(d);
        }
    }
    USendOneChar(0xFE);
    USendOneChar(0x01);
    i = 0;
    j = 0;
} */
void USendOneChar(u8 ch)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET)
        ; //μè′y·￠?í???a??
    USART_SendData(USART2, ch);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
        ; //μè′y·￠?ííê
}
//开启一次DMA
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量 
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //¹Ø±ÕDMA´«Êä 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//È·±£DMA¿ÉÒÔ±»ÉèÖÃ  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //Êý¾Ý´«ÊäÁ¿  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //¿ªÆôDMA´«Êä 
}	  

u8 UART2_DMA_Init(u8** argv, int size, u8 mode)
{
    u8 Photo[4] = { 0x01, 0xFE, 0xFE, 0x01 };
    u8 CCD[4] = { 0x02, 0xFD, 0xFD, 0x02 };
    u8 BX[4] = { 0x03, 0xFC, 0xFC, 0x03 };
    u8* temp;
    u8 i;
    switch (mode) {
    case Photo_Mode:
        temp = Photo;
        break;
    case CCD_Mode:
        temp = CCD;
        break;
    case BX_Mode:
        temp = BX;
        break;
    default:
        return 0;
        break;
    };
    USendOneChar(temp[0]);
    USendOneChar(temp[1]);
    MYDMA_Config(DMA1_Stream6, DMA_Channel_4, (u32)&USART2->DR, (u32)argv, size);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //开启传输DMA
    MYDMA_Enable(DMA1_Stream6, size); //使能DMA
    while (1) {
        if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) //中断
        {
            DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); //清除中断
            break;
        }
    }
    DMA_DeInit(DMA1_Stream6);
    USendOneChar(temp[2]);
    USendOneChar(temp[3]);
    return 1;
}