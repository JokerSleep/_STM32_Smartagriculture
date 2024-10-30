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
	//1使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//2ADC1电路使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	//3先初始化ADC1通道0 IO口 PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//模拟输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//不带上下拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	//初始化 PA0 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
 	
	//独立模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMA失能
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//4初始化
	ADC_CommonInit(&ADC_CommonInitStructure);

	//12位模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//非扫描模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	//关闭连续转换 一次转换
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//禁止触发检测，使用软件触发 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	//5ADC初始化
	ADC_Init(ADC1, &ADC_InitStructure);
	//6开启AD转换器	
	ADC_Cmd(ADC1, ENABLE);
}

unsigned short int	getAdc(void)   
{	
	//设置规则通道组
	//1ADC1,通道0,一个序列，480个机器周期,提高采样时间可以提高精确度，这是采样时间。		    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles );	
	//2开始转换
	ADC_SoftwareStartConv(ADC1);			
	//3等待转换结束
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//4获取转换结果
	return ADC_GetConversionValue(ADC1);	
}

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
float getAdcAverage(unsigned char times)
{
	//一个通道采集5次，取和 求平均值。
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
