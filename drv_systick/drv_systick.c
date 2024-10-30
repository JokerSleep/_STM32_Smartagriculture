#include "drv_systick.h"

//static u8 fs_us = 0;
//static u16 fs_ms = 0;

//Systick��ʱ���ĳ�ʼ��
//void drv_Systick_Init(u8 sysclk)
//{
//	SysTick->CTRL &= ~(0x05);
//	fs_us = sysclk / 8;
//	fs_ms = fs_us * 1000;
//	
//}

//�����ʱ��
//void drv_Systick_delayxms(u32 nms)
//{
//	u32 temp = 0;
//	SysTick->LOAD = (u32)fs_ms*nms;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= 0x01;
//	do
//	{
//		temp = SysTick->CTRL;
//	}
//	while(!(temp&(0x01<<16)));
//	SysTick->CTRL &= ~0x01;
//}

//void drv_Systick_delayms(u16 nms)
//{
//	u8 repeat = nms/540;
//	u16 remain = nms%540;
//	while(repeat)
//	{
//		drv_Systick_delayxms(540);
//		repeat--;
//	}
//	if(remain)
//		drv_Systick_delayxms(remain);
//}

//΢���ʱ��
//void drv_Systick_delayus(u32 nus)
//{
//	u32 temp = 0;
//	SysTick->LOAD = fs_us*nus;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= 0x01;
//	do
//	{
//		temp = SysTick->CTRL;
//	}
//	while(!(temp&(0x01<<16)));
//	SysTick->CTRL &= ~0x01;
//	SysTick->VAL = 0x00;
//}

void TIM7_Delayms(uint16_t time)
{
	uint32_t ms = time * 1000;
	TIM_Cmd(TIM7,ENABLE);
	while(ms--)
	{
		while(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)==RESET);
		TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	}
	TIM_Cmd(TIM7,DISABLE);
}

void TIM7_Delayus(uint32_t time)
{
	uint32_t ms = time;
	TIM_Cmd(TIM7,ENABLE);
	while(ms--)
	{
		while(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)==RESET);
		TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	}
	TIM_Cmd(TIM7,DISABLE);
}

