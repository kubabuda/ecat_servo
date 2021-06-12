#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "ecatapp.h"


int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	APP_USART_Init();
	delay_init(); 
	printf("\r\n[ESC Setup] %s \r\n", "Started");
  	ecatapp_setup();
	printf("\r\n[ESC Setup] Done, ready \r\n\n");

	while (1)
	{
		ecatapp_mainloop();
	}
}
