#ifndef _UART_P_H_
#define _UART_P_H_
#include "sys.h"

#define USART2_MAX_SEND_LEN 400
#define USART3_MAX_SEND_LEN 400

void u2_printf(char *fmt, ...);
void u3_printf(char *fmt, ...);
#endif
