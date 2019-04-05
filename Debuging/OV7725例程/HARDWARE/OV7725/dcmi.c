/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-03-09 17:34:31
 * @LastEditTime: 2019-04-05 21:55:57
 */
#include "sys.h"
#include "dcmi.h"
#include "OV7725.h"

u8 frame = 0; //֡��
//extern void jpeg_data_process(void);	//JPEG���ݴ�����

DCMI_InitTypeDef DCMI_InitStructure;

//DCMI�жϷ�����
//�ж�����ɴ���
//����ж�Ŀǰ��û�д�����
//�ǵò���һ������жϴ��Ҫ����ʱ��
void DCMI_IRQHandler(void)
{
	if (DCMI->MISR & DCMI_IT_VSYNC) //VSYNC capture
	{
		DCMI->ICR = DCMI_IT_VSYNC; //clear flag

		//		//ͼ��ɼ���ɣ�����������
		//		if(Image_raw == ImageBuff1)
		//		{
		//			Image_raw = ImageBuff2;
		//		}
		//		if(Image_raw == ImageBuff2)
		//		{
		//			Image_raw = ImageBuff1;
		//		}
		//
		//		//���ͼ������ɣ�������ͼ��ɼ�����һ��������
		//		if(ImgControl.ImgDealOk == true)
		//		{
		//			if(Image_raw == ImageBuff1)
		//			{
		//				Image = ImageBuff2;
		//			}
		//			if(Image_raw == ImageBuff2)
		//			{
		//				Image = ImageBuff1;
		//			}
		//
		//			ImgControl.ImgDealOk = false;
		//		}

		ImgControl.lines = 0;

		//ͼ�������
		if (ImgControl.ImgDealOk)
		{
			ImgControl.lines = 0; //�����
			ImgControl.ImgOk = false;

			//ImgControl.Count++;
			//64msһ��
		}
		frame++;
		//64msһ��
	}
	//1msһ��  0.5
	if (DCMI->MISR & DCMI_IT_LINE) //�н���
	{
		DCMI->ICR = DCMI_IT_LINE;

		if (ImgControl.ImgOk == false)
		{
			//1msһ��  0.5
			if (ImgControl.lines < CAMERA_H)
			{
				//1msһ��  0.5
				DMA2_Stream1->CR &= (uint32_t)~DMA_SxCR_EN;					 //DMA�ɼ�ͼ��ÿ�ı�һ�ε�ַ����Ҫ��DMAʧ�ܺ���ܸı�
				DMA2_Stream1->M0AR = (uint32_t) & (Image[ImgControl.lines]); //�µ�һ�еĵ�ַ
				DMA2_Stream1->CR |= DMA_SxCR_EN;							 //DMA2_Stream1 enable
				DCMI->CR |= DCMI_CR_CAPTURE;								 //capture enable
				ImgControl.lines++;											 //˵����һ��һ��ɨ���
			}
			else
			{
				//                //δ����
				//                //RED_TURN;
				ImgControl.ImgOk = true;
				ImgControl.ImgDealOk = true;

				//                //	taskrun_time.camera.task_time=sys_time.s*1000+sys_time.ms-taskrun_time.camera.s*1000+taskrun_time.camera.ms;
				//                //DMA2_Stream1->CR  &= (uint32_t)~DMA_SxCR_EN;//DMA_Cmd(DMA2_Stream1, DISABLE);
			}
		}
	}

	if (DCMI->MISR & DCMI_IT_FRAME)
	{
		//δ����
		DCMI->ICR = DCMI_IT_FRAME;
	}
	if (DCMI->MISR & DCMI_IT_ERR)
	{
		//δ����
		DCMI->ICR = DCMI_IT_ERR;
	}
}
//DCMIʹ��DMA2 ͨ��1��������1����ͨ��1��������7
//����ֱ��ʹ��DMA2ͨ��1��������1
//DCMI DMA����
//DMA_Memory0BaseAddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr, u16 DMA_BufferSize, u32 DMA_MemoryDataSize, u32 DMA_MemoryInc)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2ʱ��ʹ��
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
	{
	} //�ȴ�DMA2_Stream1������

	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;							//ͨ��1 DCMIͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;				//�����ַΪ:DCMI->DR
	DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;			//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;						//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //�������ݳ���:32λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;				//�洢�����ݳ��ȣ�ʡ��ģ���ǰ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							// ʹ��ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//�����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//ʡ��ģ���ﲻʹ��FIFOģʽ���Ǻ����	//FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//ʹ��ȫFIFO
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//����ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//�洢��ͻ�����δ���
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);								//��ʼ��DMA Stream

	//ע����������һ��DMA_CMD����
	/*****************************
	 * FIFOģʽÿ�����ݴճ�һ���֣�16�ֽڣ�����
	******************************/
}
//DCMI��ʼ��
//�޸����
void My_DCMI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOA B  E ʱ��
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);												//ʹ��DCMIʱ��
	//PA4/6��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10; //PA4/6/9/10   ���ù������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									  //���ù������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;								  //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									  //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);											  //��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9; // PB6/7/8/9   ���ù������
	GPIO_Init(GPIOB, &GPIO_InitStructure);											 //��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4; //PE0/1/4  ���ù������
	GPIO_Init(GPIOE, &GPIO_InitStructure);								//��ʼ��

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_DCMI);  //PA9,AF13  DCMI_D0
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_DCMI); //PA10,AF13 DCMI_D1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);  //PE0,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);  //PE1,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);  //PE4,AF13  DCMI_D4
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);  //PB6,AF13  DCMI_D5
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI);  //PB8,AF13  DCMI_D6
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);  //PB9,AF13  DCMI_D7

	DCMI_DeInit(); //���ԭ��������

	//��ʵ���������
	//����OV7620��ǵü��
	//����ʡ��ģ�����
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;	//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //8λ���ݸ�ʽ
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;			 //ˮƽͬ������	 //HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;		 //PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;	 //Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;			 //֡ͬ������	 //VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	//DCMI_ITConfig(DCMI_IT_FRAME, ENABLE); //����֡�ж�
	DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE); //������ֱ�ж�
	DCMI_ITConfig(DCMI_IT_LINE, ENABLE);  //�������ж�

	DCMI_Cmd(ENABLE);		 //DCMIʹ��
	DCMI_CaptureCmd(ENABLE); //��׽ʹ�� ��û�㶮��ʲô��˼

	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
//DMA�Զ�ת�������ֶ�ת���д���ȶ

#if 0
////////////////////////////////////////////////////////////////////////////////
//������������,��usmart����,���ڵ��Դ���

//DCMI������ʾ����
//sx,sy;LCD����ʼ����
//width,height:LCD��ʾ��Χ.
void DCMI_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	DCMI_Stop();
	LCD_Clear(WHITE);
	LCD_Set_Window(sx, sy, width, height);
	OV2640_OutSize_Set(width, height);
	LCD_SetCursor(0, 0);
	LCD_WriteRAM_Prepare(); //��ʼд��GRAM

	DMA_Cmd(DMA2_Stream1, ENABLE); //����DMA2,Stream1

	DCMI_CaptureCmd(ENABLE); //DCMI����ʹ��
}

//ͨ��usmart����,����������.
//pclk/hsync/vsync:�����źŵ����޵�ƽ����
void DCMI_CR_Set(u8 pclk, u8 hsync, u8 vsync)
{
	DCMI_DeInit(); //���ԭ��������

	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;   //����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;	//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b; //8λ���ݸ�ʽ
	DCMI_InitStructure.DCMI_HSPolarity = hsync << 6;					 //HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity = pclk << 5;					 //PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;	 //Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity = vsync << 7;					 //VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	DCMI_CaptureCmd(ENABLE); //DCMI����ʹ��
	DCMI_Cmd(ENABLE);		 //DCMIʹ��
}
#endif
