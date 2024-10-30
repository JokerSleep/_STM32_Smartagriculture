#ifndef __DRV_DHT11_H
#define __DRV_DHT11_H

#include "stm32f4xx.h"

#define    DHT11_DQ_OUT_HIGH    GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define    DHT11_DQ_OUT_LOW 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)

#define    DHT11_DQ_IN	        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)

//IO��������
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

   	
uint8_t drv_DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11  
void task_DHT11_Rst(void);
#endif
