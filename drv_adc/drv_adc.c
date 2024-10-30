#include "drv_adc.h"
#include "drv_systick.h"

uint16_t ADC_BUFF[100];
float light = 0;
uint32_t sum,sum1;

void drv_ADC1_Init(void)
{
	/*
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
//	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//168/2/4=21MHZ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	DMA_InitStruct.DMA_BufferSize = 100;
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)ADC_BUFF;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	DMA_Cmd(DMA2_Stream0,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_SoftwareStartConv(ADC1);
	*/
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	//1ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//2ADC1��·ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	//3�ȳ�ʼ��ADC1ͨ��0 IO�� PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//ģ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	//��ʼ�� PA0 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
 	
	//����ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMAʧ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//4��ʼ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	//12λģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//��ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	//�ر�����ת�� һ��ת��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//��ֹ������⣬ʹ��������� 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//�Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	//5ADC��ʼ��
	ADC_Init(ADC1, &ADC_InitStructure);
	//6����ADת����	
	ADC_Cmd(ADC1, ENABLE);
}

unsigned short int	getAdc(void)   
{	
	//���ù���ͨ����
	//1ADC1,ͨ��0,һ�����У�480����������,��߲���ʱ�������߾�ȷ�ȣ����ǲ���ʱ�䡣		    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles );	
	//2��ʼת��
	ADC_SoftwareStartConv(ADC1);			
	//3�ȴ�ת������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//4��ȡת�����
	return ADC_GetConversionValue(ADC1);	
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
float getAdcAverage(unsigned char times)
{
	//һ��ͨ���ɼ�5�Σ�ȡ�� ��ƽ��ֵ��
	long int temp_val=0;
	unsigned char  t;
	for(t=0;t<times;t++)
	{
		temp_val+=getAdc();
		TIM7_Delayms(5);
	}
	return (100/3.3)*(3.3-(temp_val/times) * (3.3/4095));
} 

/*
void drv_ADC_Value(void)
{
	uint8_t i;
	
	for(i=0;i<100;i++)
	{
		sum = sum + ADC_BUFF[i];
		TIM7_Delayms(5);
	}
	sum1 = sum/100;
//	sum1 = ADC1->DR;
	light = (100/3.3)*(3.3-(float)sum1 * (3.3/4095));
}
*/
