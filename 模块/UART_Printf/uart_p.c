#include "uart_p.h"
#include "stdarg.h"

//USART2 发送缓存
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
//USART3 发送缓存
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];

void u2_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART2_TX_BUF); //发送数据的长度
    for (j = 0; j < i; j++)                  //循环发送数据
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) //等待上次发送完成
            ;
        USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); //发送到串口2
    }
}

void u3_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART3_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART3_TX_BUF); //发送数据的长度
    for (j = 0; j < i; j++)                  //循环发送数据
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) //等待上次发送完成
            ;
        USART_SendData(USART3, (uint8_t)USART3_TX_BUF[j]); //发送到串口3
    }
}
