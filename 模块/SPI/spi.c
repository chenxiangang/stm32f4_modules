#include "spi.h"

void My_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SPI_MISO_RCC|SPI_MOSI_RCC|SPI_SCLK_RCC, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB1PeriphClockCmd(SPI_RCC, ENABLE);//ʹ��SPI2ʱ��
	
	//GPIO����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//���� 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//100MHz
	
	//��ʼ���ṹ��
	//MISO
	GPIO_InitStructure.GPIO_Pin=SPI_MISO_Pin;
	GPIO_Init(SPI_MISO_GPIO,&GPIO_InitStructure); 
	//MOSI
	GPIO_InitStructure.GPIO_Pin=SPI_MOSI_Pin;
	GPIO_Init(SPI_MOSI_GPIO,&GPIO_InitStructure);
	//SCLK
	GPIO_InitStructure.GPIO_Pin=SPI_SCLK_Pin; 
	GPIO_Init(SPI_SCLK_GPIO,&GPIO_InitStructure);
	
	//��������
	GPIO_PinAFConfig(SPI_MISO_GPIO,SPI_MISO_AF_PIN,SPI_AF);  //MISO����
	GPIO_PinAFConfig(SPI_MOSI_GPIO,SPI_MOSI_AF_PIN,SPI_AF);  //MOSI����
	GPIO_PinAFConfig(SPI_SCLK_GPIO,SPI_SCLK_AF_PIN,SPI_AF);  //SCLK����
	
	
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(SPI_RCC,ENABLE);//��λSPI2
	RCC_APB1PeriphResetCmd(SPI_RCC,DISABLE);//ֹͣ��λSPI2
	
	//Ԥ��Ƶֵ
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_21M;
	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����     ��һ�������ǵ�һ�����������ݲɼ�
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;	
	//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ   �ߵ�ƽΪ����״̬
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	//CRCֵ����Ķ���ʽ ���ͨ�ſɿ��ԣ�����1����
	SPI_InitStructure.SPI_CRCPolynomial=7;	
	//���ݳ�8λ     8λ��16λ��ѡ
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b; 
	//˫��ȫ˫��ģʽ  ��ѡ�������а�˫����
	SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex; 
	//��һλΪ�������λ
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	//����ģʽ        ��ѡ�������дӻ�
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;  
	//NSS�ź�������� ����Ƭѡ�ź�
	//Ӳ�����е㲻��
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; 
	
	//��ʼ��
	SPI_Init(SPI_Use, &SPI_InitStructure);
	
	SPI_Cmd(SPI_Use, ENABLE); //ʹ��SPI����
	
	SPI_ReadWriteByte(SPI_Use,0xff);//��Ҷ�д��֪��Ϊʲô	 
}

//SPI �ٶ�����
//��һ���������õ�SPI
//�ڶ�������SPI�ٶ�
void SPI_SetSpeed(SPI_TypeDef* SPIx , u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPIx->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPIx->CR1|=SPI_BaudRatePrescaler;	//����SPI�ٶ� 
	SPI_Cmd(SPIx,ENABLE); //ʹ��SPI
} 

//��������ԭ��
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����	
 		    
}

//дһ������
void SPI_WriteByte(SPI_TypeDef* SPIx , u8 TxData)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ��byte  ����
}

//��һ������
u8 SPI_ReadByte(SPI_TypeDef* SPIx )
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����
}