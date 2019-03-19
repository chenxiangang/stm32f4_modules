<!--
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 18:46:27
 * @LastEditTime: 2019-03-16 19:30:23
 -->
## HOW TO HOW

## 接法
- IN1 PD0
- IN2 PD1
- IN3 PD2
- IN4 PD3


### 函数接口
```
/**
 * @description: 光电对管初始化  PD0~PD3
 * @param {type} void
 * @return: void
 */
void YL_70_Init(void)
```

```
/**
 * @description: 读单个光电对管的值
 * @param {type} ch 光电对管的通道
 * @return: 高低电平
 */
u8 YL_70_Read_One(u8 ch)
```
```
/**
 * @description: 读一组光电对管
 * @param {type} group 4位长的数组
 * @return: void
 */
void YL_70_Read_All(u8 *group)
```

