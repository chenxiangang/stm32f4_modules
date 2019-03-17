#include "mynvic.h"

/*************************************** 中断初始化 *************************************/

#define UART3_ON 0
#define TIM4_ON 0
#define USART1_ON 0
#define TIM2_ON 0
#define CAN1_ON 1
/**
  * @brief  中断初始化函数.
  * @param  None
  * @retval None
  */
void My_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  Disable_Interrupts();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /***************************interrupt 1*************************/
#if TIM2_ON
  //定时器2 from timer.c
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

#if USART1_ON
  //串口1配置 from usart.c
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器
#endif

//串口3配置 from JY901_uart.c
#if UART3_ON
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //串口3中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器
#endif

//定时器4 from encoder.c
#if TIM4_ON
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

//CAN1 from CAN.c
#if CAN1_ON
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;       //接收中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   				//使能 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //次优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //主优先级1
	NVIC_Init(&NVIC_InitStructure);
#endif
  /*************************** DCMI Interrupt ****************/
  Enable_Interrupts();
}

/*************************************** System IRQ handers *************************************/

/*The mainly control timer interrupt*/
void TIM7_IRQHandler(void)
{
}

/**
	*example
	*WWDG_IRQHandler()
	*{

	*}

	*ALL IRQHandler are include in 
	#include "startup_stm32f40_41xxx.s"
	*mouse right click to jump to
	*/
//void DCMI_IRQHandler(void)
//{
//	/* Capture a flame of picture */
//	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)
//	{
//
////		/**
////			* Your blocks
////			*	Deal the picture data
////			*/
//		LED0 = 0;
//		LED1 = 0;
//		delay_ms(1000);
//
//		/* Clear the flag */
//		DCMI_ClearITPendingBit(DCMI_IT_FRAME);
////		LED0 = 1;
////		LED1 = 0;
////		delay_ms(1000);
//	}
//
//	/* Capture a line */
////	else if (DCMI_GetITStatus(DCMI_IT_LINE)==SET)
////	{
////		/* if buff if half full, DMA the BUFF to W25Q128 */
////		//W25Q128_WriteData(W25Q128_Based_Addr,&Image[W25Q128_Based_Addr%(ROW_LEN*8)],ROW_LEN);

//////		W25Q128_Based_Addr += ROW_LEN;
////		DCMI_ClearITPendingBit(DCMI_IT_LINE);
////	}
//}
