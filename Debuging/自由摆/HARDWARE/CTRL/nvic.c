#include "nvic.h"


/*************************************** 中断初始化 *************************************/

/**
  * @brief  中断初始化函数.
  * @param  None
  * @retval None
  */
void Nvic_Init(void)
{
	//NVIC_InitTypeDef NVIC_InitStructure;

	Disable_Interrupts();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/***************************interrupt 1*************************/
	
	/**
		* Your blocks
		*/
	
	/*************************** DCMI Interrupt ****************/
	
	/* NVIC initilization */
//  NVIC_InitStructure.NVIC_IRQChannel									 = DCMI_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;	
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority				 =0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd								 = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	
	/* ... */

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



