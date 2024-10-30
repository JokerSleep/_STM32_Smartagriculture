#include <stdio.h>
#include <string.h>
#include "oled.h"
#include "bmp.h"
#include "drv_systick.h"
#include "drv_key.h"
#include "drv_led.h"
#include "drv_beep.h"
#include "drv_tim.h"
#include "drv_exti.h"
#include "drv_adc.h"
#include "drv_dht11.h"
#include "drv_usart.h"
#include "FreeRTOS.h"
#include "task.h"

#define    OLED_STK_SIZE 	       128
#define    OLED_TASK_PRIO	        2
TaskHandle_t OLED_Handler;
void OLED_task(void *pvParameters);

#define    KEY_STK_SIZE 	       128
#define    KEY_TASK_PRIO	        2
TaskHandle_t KEY_Handler;
void KEY_task(void *pvParameters);

#define    ADC_STA_SIZE            128     //�����ջ��С
#define    ADC_TASK_PRIO            2      //�������ȼ�
TaskHandle_t ADCTask_Handler;              //������
void ADC_task(void *pvParameters);         //��ʼ������

#define    DHT11_STA_SIZE            128     //�����ջ��С
#define    DHT11_TASK_PRIO            2      //�������ȼ�
TaskHandle_t DHT11Task_Handler;              //������
void DHT11_task(void *pvParameters);         //��ʼ������

#define    FAN1_STA_SIZE            128     //�����ջ��С
#define    FAN1_TASK_PRIO            1      //�������ȼ�
TaskHandle_t FAN1Task_Handler;              //������
void FAN1_task(void *pvParameters);         //��ʼ������ 

#define    PWM_STA_SIZE            128     //�����ջ��С
#define    PWM_TASK_PRIO            2      //�������ȼ�
TaskHandle_t PWMTask_Handler;              //������
void PWM_task(void *pvParameters);         //��ʼ������



float guangzhao;
extern struct keys key[];
extern float light;
extern uint32_t sum1;
uint8_t ui = 0;                //��������ı�־
_Bool ui2 = 0;
_Bool LED1_flag = 0;           //LED2��״̬��־λ  0--LEDOFF   1--LEDON
_Bool BEEP_flag = 0;           //BEEP��״̬��־λ  0--BEEPOFF  1--BEEPON

uint16_t Temperature,Humidity,a,b;
uint16_t pwmVal = 0;

u8 key1_num = '*';
u8 key2_num = '*';
u8 key3_num = '*';

u8 key1_true = '1';
u8 key2_true = '2';
u8 key3_true = '3';
u8 Cnt_Error = 0;

void drv_KEY_Proc(void)
{
	if(ui == 0)
	{
		if(key[4].flag == 1)
		{
			ui = 1;
			key[4].flag = 0;
		}
	}
	if(ui == 1)
	{
		if(key[1].flag == 1)
		{
			if(key1_num == '*')
			key1_num = 48 - 1;
			if(++key1_num >= 58)
				key1_num = 48;
			key[1].flag = 0;
		}
		if(key[2].flag == 1)
		{
			if(key2_num == '*')
			key2_num = 48 - 1;
			if(++key2_num >= 58)
				key2_num = 48;
			key[2].flag = 0;
		}
		if(key[3].flag == 1)
		{
			if(key3_num == '*')
			key3_num = 48 - 1;
			if(++key3_num >= 58)
				key3_num = 48;
			key[3].flag = 0;
		}
		if(key[4].flag == 1)
		{
			if(key1_num == key1_true && key2_num == key2_true &&key3_num == key3_true)
			{
				ui = 2;
				OLED_Refresh();
				OLED_Clear();
			}
			else
			{
				Cnt_Error++;
				if(Cnt_Error == 3)
				{
					BEEP_Circle();
					Cnt_Error = 0;
				}
				else
				{
					BEEP_OFF();
				}
			}
			key[4].flag = 0;
			key1_num = '*';
			key2_num = '*';
			key3_num = '*';
		}
	}
	if(ui == 2)
	{
		if(key[1].flag == 1)
		{
			LED1_flag =! LED1_flag;
			key[1].flag = 0;
		}
		
		if(key[2].flag == 1)
		{
			BEEP_flag =! BEEP_flag;
			key[2].flag = 0;
		}
		if(key[4].flag)
		{
			ui2 =! ui2;
			key[4].flag = 0;
		}
	}
	
	
}

void drv_LED_PWM(void)
{
	if(guangzhao > 70)
	{
		TIM_SetCompare1(TIM1,0);
		vTaskDelay(5);
	}
	else if(guangzhao > 50 && guangzhao < 70)
	{
		TIM_SetCompare1(TIM1,100);
		vTaskDelay(5);
	}
	else if(guangzhao > 20 && guangzhao < 50)
	{
		TIM_SetCompare1(TIM1,500);
		vTaskDelay(5);
	}
	else if(guangzhao < 20)
	{
		TIM_SetCompare1(TIM1,1000);
		vTaskDelay(5);
	}
}

void drv_LCD_Proc(void)
{
	if(ui == 0)
	{
		OLED_ShowChinese(22,12,0,16,1);//��
		OLED_ShowChinese(36,12,1,16,1);//��
		OLED_ShowChinese(50,12,2,16,1);//Զ
		OLED_ShowChinese(64,12,3,16,1);//��
		OLED_ShowChinese(80,12,4,16,1);//�� 
		OLED_ShowChinese(94,12,5,16,1);//ɳ
		
		OLED_ShowChinese(8,36,6,16,1);   //С
		OLED_ShowChinese(22,36,7,16,1);  //��
		OLED_ShowChinese(36,36,8,16,1);  //��
		OLED_ShowChinese(50,36,9,16,1);  //��
		OLED_ShowChinese(64,36,10,16,1); //ũ
		OLED_ShowChinese(80,36,11,16,1); //ҵ
		OLED_ShowChinese(94,36,12,16,1); //ϵ
		OLED_ShowChinese(108,36,13,16,1);//ͳ
		OLED_Refresh();
//		TIM7_Delayms(1000);
		OLED_Clear();
//		OLED_ScrollDisplay(6,4,1);
	}
	
	else if(ui == 1)
	{
		char text[20];
		
		OLED_ShowChinese(8,0,6,16,1);   //С
		OLED_ShowChinese(22,0,7,16,1);  //��
		OLED_ShowChinese(36,0,8,16,1);  //��
		OLED_ShowChinese(50,0,9,16,1);  //��
		OLED_ShowChinese(64,0,10,16,1); //ũ
		OLED_ShowChinese(80,0,11,16,1); //ҵ
		OLED_ShowChinese(94,0,12,16,1); //ϵ
		OLED_ShowChinese(108,0,13,16,1);//ͳ
		
		OLED_ShowString(48,16,"K1:",16,1);
		sprintf(text,"%c",key1_num);
		OLED_ShowString(73,16,(u8 *)text,16,1);
		
		OLED_ShowString(48,32,"K2:",16,1);
		sprintf(text,"%c",key2_num);
		OLED_ShowString(73,32,(u8 *)text,16,1);
		
		OLED_ShowString(48,48,"K3:",16,1);
		sprintf(text,"%c",key3_num);
		OLED_ShowString(73,48,(u8 *)text,16,1);
		
		OLED_Refresh();
		OLED_Clear();
		
	}
	else if(ui2 == 0)
	{
		char text[20];
		OLED_ShowChinese(10,0,0,16,1); //��
		OLED_ShowChinese(28,0,1,16,1); //��
		OLED_ShowChinese(46,0,2,16,1); //Զ
		OLED_ShowChinese(64,0,3,16,1); //��
		OLED_ShowChinese(82,0,4,16,1); //��
		OLED_ShowChinese(100,0,5,16,1);//ɳ
		OLED_ShowString(121,48,"1",16,1);
		
		if(LED1_flag)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:ON",16,1);
		}
		else
		{	
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:OFF",16,1);
		}
		if(BEEP_flag)
		{
			BEEP_ON();
			OLED_ShowString(0,32,"BEEP:ON",16,1);
		}
		else
		{
			BEEP_OFF();
			OLED_ShowString(0,32,"BEEP:OFF",16,1);
		}
		OLED_ShowString(0,48,"LIGHT:",16,1);
		sprintf(text,"%.1fLUX",guangzhao);
		OLED_ShowString(49,48,(u8 *)text,16,1);
		
		OLED_Refresh();
//		TIM7_Delayms(1000);
		OLED_Clear();
		
	}
	else if(ui2 == 1)
	{
		
		OLED_ShowChinese(10,0,0,16,1); //��
		OLED_ShowChinese(28,0,1,16,1); //��
		OLED_ShowChinese(46,0,2,16,1); //Զ
		OLED_ShowChinese(64,0,3,16,1); //��
		OLED_ShowChinese(82,0,4,16,1); //��
		OLED_ShowChinese(100,0,5,16,1);//ɳ
		OLED_ShowString(121,48,"2",16,1);
		char text[20];
		OLED_ShowString(0,32,"Temp:",16,1);
		sprintf(text,"%d.%dC",Temperature>>8,Temperature&0xff);
		OLED_ShowString(41,32,(u8 *)text,16,1);
		
		OLED_ShowString(0,48,"Humi:",16,1);
		sprintf(text,"%d.%dAH",Humidity>>8,Humidity&0xff);
		OLED_ShowString(41,48,(u8 *)text,16,1);
		
		OLED_Refresh();
//		TIM7_Delayms(1000);
		OLED_Clear();
	}
	
}

void drv_FAN_Proc(void)
{
	if(Temperature>>8 > 30)
	{
		FAN_ON();
		BEEP_Circle();
	}
	else
	{
		FAN_OFF();
		BEEP_OFF();
	}
	
	if(Humidity>>8 > 80)
	{
		FAN_ON();
		LED3_Circle();
	}
	else
	{
		FAN_OFF();
		BEEP_OFF();
	}
}

void drv_ESP8266_SendByte(void)
{
	
	TIM7_Delayms(1000);
	drv_USART_SendStr(USART2,"AT+RST\r\n",strlen("AT+RST\r\n"));
	TIM7_Delayms(500);
	drv_USART_SendStr(USART2,"AT\r\n",strlen("AT\r\n"));
	TIM7_Delayms(500);
	drv_USART_SendStr(USART2,"AT+CWMODE=2\r\n",strlen("AT+CWMODE=2\r\n"));
	TIM7_Delayms(500);
//	drv_USART_SendStr(USART2,"AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"));
//	TIM7_Delayms(1000);
	drv_USART_SendStr(USART2,"AT+CWSAP=\"ESP_12S\",\"12666666\",5,4\r\n",strlen("AT+CWSAP=\"ESP_12S\",\"12666666\",5,4\r\n"));
	TIM7_Delayms(1000);
//	drv_USART_SendStr(USART2,"AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n",strlen("AT+CWJAP=\"hqyjcs\",\"hqyj2022\"\r\n"));
	drv_USART_SendStr(USART2,"AT+CIPAP=\"192.168.12.14\"\r\n",strlen("AT+CIPAP=\"192.168.12.14\"\r\n"));
	TIM7_Delayms(5000);
	drv_USART_SendStr(USART2,"AT+CIFSR\r\n",strlen("AT+CIFSR\r\n"));
	TIM7_Delayms(1000);
	
	drv_USART_SendStr(USART2,"AT+CIPMUX=1\r\n",strlen("AT+CIPMUX=1\r\n"));
	TIM7_Delayms(1000);
//	drv_USART_SendStr(USART2,"AT+CIPSTART=\"TCP\",\"192.168.50.114\",9090\r\n",strlen("AT+CIPSTART=\"TCP\",\"192.168.50.114\",9090\r\n"));
//	TIM7_Delayms(1000);
	drv_USART_SendStr(USART2,"AT+CIPSERVER=1,9090\r\n",strlen("AT+CIPSERVER=1,9090\r\n"));
	TIM7_Delayms(1000);
//	drv_USART_SendStr(USART2,"AT+CIPAP=\"192.168.50.114\"\r\n",strlen("AT+CIPAP=\"192.168.50.114\"\r\n"));
//	TIM7_Delayms(1000);
	
//	drv_USART_SendStr(USART2,"AT+CIPSTART=\"TCP\",\"192.168.50.205\",9090\r\n",strlen("AT+CIPSTART=\"TCP\",\"192.168.50.205\",9090\r\n"));
//	drv_USART_SendStr(USART2,"AT+CIPSTART=\"TCP\",\"192.168.90.136\",9090\r\n",strlen("AT+CIPSTART=\"TCP\",\"192.168.90.136\",9090\r\n"));
//	TIM7_Delayms(3000);
//	drv_USART_SendStr(USART2,"AT+CIPSEND=6\r\n",strlen("AT+CIPSEND=6\r\n"));
//	TIM7_Delayms(500);
//	drv_USART_SendStr(USART2,"HEJIAN\r\n",strlen("HEJIAN\r\n"));
//	TIM7_Delayms(500);
}


int main()
{
	drv_TIM7_Init();
//	drv_Systick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	drv_DHT11_Init();
	
	drv_ADC1_Init();
	OLED_Init();
	
	drv_KEY_Init();
 	drv_TIM6_Init();		
	drv_LED_Init();
	drv_BEEP_Init();
	
	drv_TIM1_Init();
	
	drv_USART1_Init();
	drv_USART2_Init();
	
	OLED_ShowChinese(8,12,14,16,1);    //ϵ
	OLED_ShowChinese(24,12,15,16,1);   //ͳ
	OLED_ShowChinese(40,12,16,16,1);   //��
	OLED_ShowChinese(56,12,17,16,1);   //��
	OLED_ShowChinese(72,12,18,16,1);   //��
	OLED_ShowChinese(88,12,19,16,1);   //ʼ
	OLED_ShowChinese(104,12,20,16,1);  //��
	OLED_ShowChinese(40,36,21,16,1);   //��
	OLED_ShowChinese(56,36,22,16,1);   //��
	OLED_ShowChinese(72,36,23,16,1);   //��
	OLED_Refresh();
	OLED_Clear();
	
	drv_ESP8266_SendByte();
	
	ui = 0 ;
	
	//������ʼ����
	xTaskCreate((TaskFunction_t)OLED_task,
				(const char *  )"OLED_task",
				(uint16_t      )OLED_STK_SIZE,
				(void *        )NULL,
				(UBaseType_t   )OLED_TASK_PRIO,
				(TaskHandle_t *)&OLED_Handler);
				
	xTaskCreate((TaskFunction_t)KEY_task,
				(const char *  )"KEY_task",
				(uint16_t      )KEY_STK_SIZE,
				(void *        )NULL,
				(UBaseType_t   )KEY_TASK_PRIO,
				(TaskHandle_t *)&KEY_Handler);
	
	xTaskCreate((TaskFunction_t)ADC_task,
				(const char *  )"ADC_task",
				(uint16_t      )ADC_STA_SIZE,
				(void *        )NULL,
				(UBaseType_t   )ADC_TASK_PRIO,
				(TaskHandle_t *)&ADCTask_Handler);
	
	
	xTaskCreate((TaskFunction_t)DHT11_task,
				(const char *  )"DHT11_task",
				(uint16_t      )DHT11_STA_SIZE,
				(void *        )NULL,
				(UBaseType_t   )DHT11_TASK_PRIO,
				(TaskHandle_t *)&DHT11Task_Handler);
	
				
	xTaskCreate((TaskFunction_t)FAN1_task,
				(const char *  )"FAN1_task",
				(uint16_t      )FAN1_STA_SIZE,
				(void *        )NULL,
				(UBaseType_t   )FAN1_TASK_PRIO,
				(TaskHandle_t *)&FAN1Task_Handler);
					
	xTaskCreate((TaskFunction_t)PWM_task,
				(const char *  )"PWM_task",
				(uint16_t      )PWM_STA_SIZE,
				(void *        )NULL,
				(UBaseType_t   )PWM_TASK_PRIO,
				(TaskHandle_t *)&PWMTask_Handler);
	//�����������
	vTaskStartScheduler();
	
}

//OLED������ 
void OLED_task(void *pvParameters)
{
	while(1)
	{
		drv_LCD_Proc();
		vTaskDelay(500);
	}
}

void KEY_task(void *pvParameters)
{
	while(1)
	{
		drv_KEY_Proc();
		vTaskDelay(500);
	}
}

void ADC_task(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();
		guangzhao = getAdcAverage(4);
		taskEXIT_CRITICAL(); 
		vTaskDelay(500);
	}
}

void DHT11_task(void *pvParameters)
{
	
	while(1)
	{
		taskENTER_CRITICAL();
		DHT11_Read_Data(&Temperature,&Humidity);
		taskEXIT_CRITICAL(); 
		vTaskDelay(500);
	}
	
}

void FAN1_task(void *pvParameters)
{
	while(1)
	{
		drv_FAN_Proc();
		vTaskDelay(500);
	}
}

void PWM_task(void *pvParameters)
{
	while(1)
	{
		drv_LED_PWM();
		vTaskDelay(500);
	}
}
