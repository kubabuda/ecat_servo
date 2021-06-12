#ifndef HSE_VALUE
  /*!< Value of the External oscillator in Hz
   * To work, needs to be declared before SPL "stm32f4xx.h"
   */
  #define HSE_VALUE    ((uint32_t)8000000) 

#endif // HSE_VALUE

#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "ECAT_App.h"


volatile uint32_t howLongUs;

int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	Delay_init();
    ECAT_AppSetup();
	
	while (1)
	{
		// ECAT_PDIcommBenchmark_us();
		ECAT_Application();
	}
}