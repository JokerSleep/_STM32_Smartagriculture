#ifndef __DRV_SYSTICK_H
#define __DRV_SYSTICK_H

#include "stm32f4xx.h"

//void drv_Systick_Init(u8 sysclk);
//void drv_Systick_delayxms(u32 nms);
//void drv_Systick_delayms(u16 nms);
//void drv_Systick_delayus(u32 nus);

void TIM7_Delayms(uint16_t time);
void TIM7_Delayus(uint32_t time);

#endif

