#include "delay.h"


static uint32_t ticksPerUs = 168;  // max STM32F40x freq by default

volatile uint32_t timer_ms = 0;


void SysTick_Handler()
{
    timer_ms++;
    /* Reset tick counter */
    DWT->CYCCNT = 0;
}


uint32_t Delay_init(void) {
	uint32_t c;
    
    /* Calculate ticks per us factor */
    ticksPerUs = (SystemCoreClock / 1000000);
	
    /* Enable TRC */
    CoreDebug->DEMCR &= ~0x01000000;
    CoreDebug->DEMCR |=  0x01000000;
	
    /* Enable counter */
    DWT->CTRL &= ~0x00000001;
    DWT->CTRL |=  0x00000001;
	
    /* Reset counter */
    DWT->CYCCNT = 0;
	
	/* Check if DWT has started */
	c = DWT->CYCCNT;
	
	/* 2 dummys */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	
	/* Return difference, if result is zero, DWT has not started */
	return (DWT->CYCCNT - c);
}


uint32_t micros() 
{
    return timer_ms * 1000 + ((DWT->CYCCNT / ticksPerUs) % 1000);
}


uint32_t millis() 
{
    return timer_ms;
}


void delay_ms(int time)
{
    uint32_t timer_end = millis() + time;
    while(millis() < timer_end);
}
