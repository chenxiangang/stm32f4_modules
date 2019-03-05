<!--
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-05 12:51:25
 * @LastEditTime: 2019-03-05 13:05:08
 -->
## HOW TO USE

### 接法
- PA4 EN1
- PA1 EN1_IN1
- PA2 EN1_IN2
- PA5 EN2
- PA3 EN2_IN1
- PA4 EN2_IN2

### 函数接口
- void BTN7971_EN_Init(int channel);  
 初始化EN口，1是EN1，2是EN2，3是全部   默认使能口高电平
- void BTN7971_EN1_PWM_Init(void);  
初始化EN1的两个PWM输入    默认输出0
- void BTN7971_EN2_PWM_Init(void);  
  初始化EN2的两个输入				默认输出0
- void BTN7971_Init(void);          
只是上面3个函数的包含，按需求修改
- void contrl_speed(int speed,int ch);  
speed为速度输入,  ch通道选择，1位EN1，2为EN2