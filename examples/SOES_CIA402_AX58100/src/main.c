#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "delay.h"
#include "usart.h"

#include "esc.h"
#include "ecat_slv.h"
#include "ecatapp.h"


uint32_t ecatapp_benchmark_us(void);

static uint8_t sync0_irq_flag = 0;

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        sync0_irq_flag = 1;
    }
}

static uint8_t pdi_irq_flag = 0;

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
        pdi_irq_flag = 1;
    }
}


int main(void)
{
  	SysTick_Config(SystemCoreClock / 1000);
	APP_USART_Init();
	delay_init(); 
    STM_EVAL_PBInit(BUTTON_MODE_GPIO);
	printf("\r\n[ESC Setup] %s \r\n", "Strted");
    ecatapp_init();
    printf("\r\n[ESC Setup] Done, ready \r\n\n");
    

	while (1)
	{
        // ecatapp_benchmark_us();
        ecatapp_loop();
	}
}

void ecatapp_loop(void)
{
    // stack in mixed mode
    if (sync0_irq_flag) {
        ESC_updateALevent();        
        DIG_process (DIG_PROCESS_APP_HOOK_FLAG | DIG_PROCESS_INPUTS_FLAG);
        sync0_irq_flag = 0;
    }
    if (pdi_irq_flag) {
        ESC_updateALevent();
        if (ESCvar.dcsync) {
            DIG_process (DIG_PROCESS_OUTPUTS_FLAG);    
        } else {
            DIG_process (DIG_PROCESS_OUTPUTS_FLAG | DIG_PROCESS_APP_HOOK_FLAG | DIG_PROCESS_INPUTS_FLAG);
        }
        pdi_irq_flag = 0;
    } else {
        // ecat_slv_worker(ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
        //                 | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1);
        ecat_slv_poll();
        DIG_process(DIG_PROCESS_WD_FLAG);
    }
}

uint32_t ecatapp_benchmark_us()
{
    // benchmark start
    stopwatch_t st;
    stopwatch_start(&st);
    
    ecatapp_loop();
 
    // benchmark stop
    volatile uint32_t elapsed_us = stopwatch_now_us(&st);
    static volatile uint32_t hiscore = 0;

    if (elapsed_us > hiscore) 
    {
        hiscore = elapsed_us;
    }

    if (STM_EVAL_IsPressed())
    {
        printf("PDI irq = %s \r\n", HEX4B(pdi_irq_flag));
    }

    return elapsed_us;
}
