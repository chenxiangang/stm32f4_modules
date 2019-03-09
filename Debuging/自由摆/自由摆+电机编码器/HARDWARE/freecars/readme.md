## How to use
- 复制freecars文件夹到工程目录，添加头文件路径
- 工程下新建freecars目录，添加[FreeCars_isr.c](https://github.com/while0l1/stm32f4_modules/blob/master/%E6%A8%A1%E5%9D%97/freecars/FreeCars_isr.c)和 [FreeCars_uart.c](https://github.com/while0l1/stm32f4_modules/blob/master/%E6%A8%A1%E5%9D%97/freecars/FreeCars_uart.c)
- main函数里面添加
  `#include "FreeCars_uart.h"`
- 由于FreeCars的调试功能用的是串口1，需要用到中断，与正点的原本的串口1冲突了，可以考虑删掉正点的

- 把需要发送的数据用push发送，第一个参数是通道，第二个是数据。**注意** ：发送完毕后需要调用`uSendOnePage();`

- _isr里面有一个a用来测试控制功能的，可以在main里面声明一个全局变量a来进行测试，也可以删掉 