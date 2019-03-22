#include "mynvic.h"

/*************************************** �жϳ�ʼ�� *************************************/

#define USART3_ON 1
#define USART1_ON 1
#define TIM1_ON 1
#define TIM2_ON 1
#define TIM3_ON 1
#define TIM4_ON 1
#define TIM5_ON 1
/**
  * @brief  �жϳ�ʼ������.
  * @param  None
  * @retval �Ѹ����жϵ�����д��ע�ͣ�Ȼ��ź��������ȼ�
  */
void My_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    Disable_Interrupts();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /***************************interrupt 1*************************/

#if USART1_ON
    //����1���� from usart.c
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif

//����3���� from JY901_uart.c
#if USART3_ON
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //����3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif

#if TIM1_ON
    //��ʱ��2 ���ֱ�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn; //�ⲿ�ж�1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC
#endif

#if TIM2_ON
        //��ʱ��2 ���ֱ�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#if TIM3_ON
    //��ʱ��3 pwm
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

//��ʱ��4 from encoder.c ���ֱ�����
#if TIM4_ON
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#if TIM5_ON
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
