#include "nrf.h"


u8 USART2_RX_BUF[USART2_REC_LEN]; //接收数组
u8 USART2_TX_BUF[30];             //发送数组
u16 USART2_RX_STA = 0;

//bound 波特率设置
void NRF_Init(int bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能GPIOD时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2); //GPIOD5复用为USART2 TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); //GPIOD6复用为USART2 RX

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOD5与GPIOD6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);                 //初始化PD5，PD6

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     //复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //USART2 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                                     //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式
    USART_Init(USART2, &USART_InitStructure);                                       //初始化串口2

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE); //使能串口2

    USART_ClearFlag(USART2, USART_FLAG_TC); //默认为接收模式

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}
u8 NRF_Send_One(u8 data)
{
    u8 flag = 0;
    while (NRF_RST)
        flag = 1; //等待
    USART_SendData(USART2, data);
    return flag;
}

int NRF_Send(u8 *buf, int len, u8 mode)
{
    int i;
    u8 Photo[4] = {0x01, 0xFE, 0xFE, 0x01};
    u8 CCD[4] = {0x02, 0xFD, 0xFD, 0x02};
    u8 BX[4] = {0x03, 0xFC, 0xFC, 0x03};
    u8 *temp;
    switch (mode)
    {
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
    };
    /* for (i = 0; i < 2; i++)
    {
        while (NRF_RST)
        {
        };
        USART_SendData(USART2, *(temp + i));
    }
    for (i = 0; i < len; i++)
    {
        while (NRF_RST)
        {
        };
        USART_SendData(USART2, *(buf + i));
    }
    for (i = 2; i < 4; i++)
    {
        while (NRF_RST)
        {
        };
        USART_SendData(USART2, *(temp + i));
    }
    while(!NRF_RST)   //填充
    {
        USART_SendData(USART2, 0x00);
    } */
    NRF_Send_One(temp[0]);
    NRF_Send_One(temp[1]);
    for (i = 0; i < len; i++)
        NRF_Send_One(buf[i]);
    NRF_Send_One(temp[2]);
    i = NRF_Send_One(temp[3]);
    while (!i) //未满30
    {
        i = NRF_Send_One(0x00);
    }
    return 1;
}

void USART2_IRQHandler(void) //串口1中断服务程序
{
    u8 Res;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART2); //(USART1->DR);	//读取接收到的数据

        if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART2_RX_STA & 0x4000) //接收到了0x0d
            {
                if (Res != 0x0a)
                    USART2_RX_STA = 0; //接收错误,重新开始
                else
                    USART2_RX_STA |= 0x8000; //接收完成了
            }
            else //还没收到0X0D
            {
                if (Res == 0x0d)
                    USART2_RX_STA |= 0x4000;
                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
                    USART2_RX_STA++;
                    if (USART2_RX_STA > (USART2_REC_LEN - 1))
                        USART2_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
    {
    } //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = chx;                                    //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = par;                         //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mar;                            //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = ndtr;                                //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //初始化DMA Stream
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

//设置完后一次传输的字节量
void NRF_DMA_Send(int size)
{
    while (NRF_RST)
        ;                                          //等待传输完成
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //开启传输DMA
    MYDMA_Enable(DMA1_Stream6, size);              //使能DMA
    while (1)
    {
        if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) //中断
        {
            DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); //清除中断
            break;
        }
    }
    DMA_DeInit(DMA1_Stream6);
}

//检查一下
int NRF_DMA(u8 argv[][CAMERA_W], int size, u8 mode)
{
    //修改
    u8 Photo[4] = {0x01, 0xFE, 0xFE, 0x01};
    u8 CCD[4] = {0x02, 0xFD, 0xFD, 0x02};
    u8 BX[4] = {0x03, 0xFC, 0xFC, 0x03};
    u8 *temp;
    switch (mode)
    {
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
    };
    MYDMA_Config(DMA1_Stream6, DMA_Channel_4, (u32)&USART2->DR, (u32)temp, 4); //设置通信协议头部
    NRF_DMA_Send(4);
		DMA_DeInit(DMA1_Stream6); 
    MYDMA_Config(DMA1_Stream6, DMA_Channel_4, (u32)&USART2->DR, (u32)argv, size); //通信协议主体
		NRF_DMA_Send(26);
		size-=26;
    while(size>0)
    {
        if(size>=30)
        {
            NRF_DMA_Send(30);
            size-=30;
        }
        else
        {
            NRF_DMA_Send(size);
						size=0;
        }
        
    }
    DMA_DeInit(DMA1_Stream6); //关闭DMA
    return 1;
}
