1、移植9250时注意需要复制以下文件夹到相应位置：
HARDWARE/LedBuz
HARDWARE/MPU9250
HARDWARE/Mymath
USER/include

2、在工程里添加以上头文件路径

3、在工程中添加JY901_uart.c和MyMath.c

4、在main.c里面添加
#include "JY901_uart.h"
#include "include.h"
FLAG_Typedef     flag;

5、初始化串口3，波特率115200
usart3_init(115200);
