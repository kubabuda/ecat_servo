#ifndef __DELAY_H__
#define  __DELAY_H__

#include "stm32f4xx.h"


#define MAX_FREQ_TICKS_PER_US   168


void SysTick_Handler();


uint32_t delay_init(void);
void delay_ms(int time);
uint32_t millis();


typedef struct stopwatch_t
{ 
    uint32_t start;
} stopwatch_t;


// works with 1 tick resolution for up to 25.5 seconds measurements at 168 MHz
void stopwatch_start(stopwatch_t *);
uint32_t stopwatch_now_us(stopwatch_t *);

int8_t test_stopwatch();

#endif // __DELAY_H__
