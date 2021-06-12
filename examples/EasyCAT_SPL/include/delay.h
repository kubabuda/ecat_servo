#ifndef __DELAY_H__
#define  __DELAY_H__

#include "stm32f4xx.h"

uint32_t Delay_init(void);

void SysTick_Handler();

void delay_ms(int time);

uint32_t millis();
uint32_t micros();

#endif // __DELAY_H__
