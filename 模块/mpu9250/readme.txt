1����ֲ9250ʱע����Ҫ���������ļ��е���Ӧλ�ã�
HARDWARE/LedBuz
HARDWARE/MPU9250
HARDWARE/Mymath
USER/include

2���ڹ������������ͷ�ļ�·��

3���ڹ��������JY901_uart.c��MyMath.c

4����main.c�������
#include "JY901_uart.h"
#include "include.h"
FLAG_Typedef     flag;

5����ʼ������3��������115200
usart3_init(115200);
