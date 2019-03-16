<!--
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-16 19:57:48
 * @LastEditTime: 2019-03-16 20:01:46
 -->
## HOW TO USE

## 接法
- 输入捕获 PA0


### 接口函数
```
  void TIM5_CH1_Cap_Init(u16 psc, u16 arr)
```


### 读时间例程
```
void TIM5_CH1_Cap_Init(84-1, 0xFFFFFFF);
```
```
//计高电平时间
//返回高电平的时间
u32 Time_Counter()
{
    u32 counter = 0;
    if (TIM5CH1_CAPTURE_STA & 0X80) //捕捉到了高电平
    {
        counter = TIM5CH1_CAPTURE_STA & 0X3F;
        counter *= 0XFFFFFFFF;          //溢出时间总合 top_arr的值
        counter += TIM5CH1_CAPTURE_VAL; //等到总的高电平时间
        TIM5CH1_CAPTURE_STA = 0;        //开启下次捕获
    }
    return counter;
}
```