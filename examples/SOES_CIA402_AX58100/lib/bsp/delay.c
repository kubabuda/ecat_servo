#include "delay.h"


static uint32_t ticksPerUs = MAX_FREQ_TICKS_PER_US;  // max STM32F40x freq by default
volatile uint32_t timer_ms = 0;


void SysTick_Handler()
{
    timer_ms++;
}


uint32_t delay_init(void) {
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


uint32_t millis() 
{
    return timer_ms;
}


void delay_ms(int time)
{
    uint32_t timer_end = millis() + time;
    while(millis() < timer_end);
}


void stopwatch_start(stopwatch_t * stopwatch)
{
    stopwatch->start = DWT->CYCCNT;
}


uint32_t stopwatch_now_us(stopwatch_t * stopwatch)
{
    return (DWT->CYCCNT - stopwatch->start) / ticksPerUs;
}


int8_t test_stopwatch() {
    delay_init();
    stopwatch_t st01, st02;

    stopwatch_start(&st01);
    delay_ms(1);
    stopwatch_start(&st02);
    delay_ms(1);
    volatile uint32_t elapsed_02_us = stopwatch_now_us(&st02);
    delay_ms(1);
    volatile uint32_t elapsed_01_us = stopwatch_now_us(&st02);

    if (elapsed_02_us >= elapsed_01_us)
    {
        return -1;
    } 
    else
    {
        return 0;
    }    
}
