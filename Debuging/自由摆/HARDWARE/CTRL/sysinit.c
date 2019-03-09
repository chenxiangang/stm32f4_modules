#include "sysinit.h"



/**
  * @brief  初始化系统设备.
  * @param  None
  * @retval None
  */
void System_Init(void)
{
	/**************************** 系统软件初始化 ***************************/
	delay_init(168);
	uart_init(115200);
	/****************************** 模块初始化 *****************************/
	
	/**
		* Your blocks
		*/
	
	
}


