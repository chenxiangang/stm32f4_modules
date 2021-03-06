#ifndef __LEDBUZ_H__
#define __LEDBUZ_H__

#include "sys.h"
/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define macLED1_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define macLED_AHBxClock_FUN  RCC_AHB1PeriphClockCmd     /*几个脚共用总线*/      // 注
#define macLED1_GPIO_CLK 	    RCC_AHB1Periph_GPIOA		/* GPIO端口时钟 */
#define macLED1_GPIO_PIN		  GPIO_Pin_11			          /* 连接到SCL时钟线的GPIO */

#define macLED2_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define macLED2_GPIO_CLK 	    RCC_AHB1Periph_GPIOA		/* GPIO端口时钟 */
#define macLED2_GPIO_PIN		  GPIO_Pin_12			          /* 连接到SCL时钟线的GPIO */


#define macBUZ_GPIO_PORT    	GPIOE			              /* GPIO端口 */
#define macBUZ_GPIO_CLK 	    RCC_AHB1Periph_GPIOE		/* GPIO端口时钟 */
#define macBUZ_GPIO_PIN		    GPIO_Pin_10		          /* 连接到SCL时钟线的GPIO */

//蜂鸣器
#define BUZ_ON  GPIO_SetBits  (macBUZ_GPIO_PORT, macBUZ_GPIO_PIN)
#define BUZ_OFF GPIO_ResetBits(macBUZ_GPIO_PORT, macBUZ_GPIO_PIN)


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 带参宏，可以像内联函数一样使用 */          // 调用举例  LED1( ON );		
#define LED1(a)	if (a)	\
					GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN)


typedef enum
{
	BZ_Ready,//初始化完成
	BZ_OK,//任务完成
	BZ_Warning,
	BZ_Norm,//待机，不叫
	BZ_Key,//按键按下
}BUZ_Enum;
		
typedef enum
{
	LED_Norm,//系统正常工作
	LED_Warning,
	LED_OK,
	//LED_KEY_effect,
}LED_Enum;


typedef enum
{
	SYS_Ready,//系统初始化完成
	SYS_Norm, //系统正常工作
	SYS_Warning,//出错警报
	SYS_OK,//某任务完成
	SYS_Key,
}SYS_Enum;


extern BUZ_Enum BZ_State;
extern LED_Enum LED_State;
extern SYS_Enum SYS_State;
					
extern int bz_cnt;
extern int led_cnt;

void bz_ready(void);
void bz_ok(void);
void bz_warning(void);
void led_buz_Init(void);
void bz_work(int state);
void bz_led_work(int state);

void led_norm(void);
void led_warning(void);
void led_ok(void);
void led_work(int state);

void key_effect(void);

#endif
