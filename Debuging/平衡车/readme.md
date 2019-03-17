## 2019年WHU校赛平衡车
软件模块  
- freecars(上位机)
- pid(控制算法)

硬件模块  
- jy901(陀螺仪)
- 带编码器电机x2
- 电机驱动
- 光电对管
- 电池
- F4VET6

需要用到的硬件资源  
- UART1 波特率115200，用作FreeCars调试，完成后改为蓝牙远程控制
- UART3 波特率115200，用作jy901陀螺仪的输入串口
- TIM2  读电机编码器
- TIM3  开启中断，大概2ms进入一次，定时执行控制任务：读取传感器数据；进行pid控制
- TIM4  读电机编码器  

文件目录说明  
- freecars---------------------上位机  
- HARDWARE  
  CTRL-------------------------中断优先级配置
  - mynvic.h
  - mynvic.c
  - control.c--------------定时器3中断函数，整个系统的控制
  - control.h  
  
  mpu9250--------------------陀螺仪初始化，读数  
  - include
  - LedBuz
  - MPU9250-------------包含串口3的初始化
  - Mymath  

  TIMER-----------------------定时器2，3，4初始化
  - timer.c
  - timer.h  

  ENCODER
  - encoder.c
  - encoder.h

  OLED
  - oled.c
  - oled.h

  pid
  - pid.c
  - pid.h


