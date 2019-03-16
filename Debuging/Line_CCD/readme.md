<!--
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-15 23:04:37
 * @LastEditTime: 2019-03-15 23:05:53
 -->
## HOW TO USE

## 接法
- CLK   PC1 
- SI    PC2
- AO    PC3

## 工程构建
- 把`Line_CCD`复制到工程目录下
- 在工程中添加.c文件 `stm32f4xx_adc.c`和`stm32f4xx_dma.c`
- 在工程中添加`Line_CCD`的头文件路径
- 在`main.c`里面添加`CCD.h`
- 先加入`CCD_Init_ND()`初始化
- 直接可以读到128个值
  ```
  for(a=0; a<128; a++)
  {
	  printf("%d ",CCD_Value[a]);
  }
    ```