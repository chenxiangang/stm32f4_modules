## 介绍
    用SSI16384来测量摆杆摆动的角度，用电机编码器来测量电机转动的角度，然后就调调调?

## 存在的问题
- 在摆动一段时间后，电机的恢复到的初始位置出现偏差，虽然上位机显示的角度没有变?  
  可能的原因:电机编码器测量的**最大脉冲不准确**，可以在[encoder.h](https://github.com/while0l1/stm32f4_modules/blob/master/Debuging/%E8%87%AA%E7%94%B1%E6%91%86/HARDWARE/ENCODER/encoder.h)里进行修改