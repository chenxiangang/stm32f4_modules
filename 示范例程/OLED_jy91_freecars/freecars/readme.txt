1、复制freecars文件夹到工程目录，添加头文件路径
2、工程下新建freecars目录，添加FreeCars_isr.c和 FreeCars_uart.c
3、main函数里面添加
#include "FreeCars_uart.h"
4、由于FreeCars的调试功能用的是串口1，需要用到中断，与正点的原本的串口1冲突了，可以考虑删掉正点的

5、把需要发送的数据用push发送，第一个参数是通道，第二个是数据。
	注意！！！ ：发送完毕后需要调用uSendOnePage(); 