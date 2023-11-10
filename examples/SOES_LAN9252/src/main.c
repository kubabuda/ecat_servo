#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "delay.h"
#include "usart.h"

#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"


/* CANopen Object Dictionary */
_Objects    Obj;


/* Application hook declaration */
void ecatapp();


/* SOES configuration */
static esc_cfg_t config = { 
    .user_arg = "/dev/lan9252",
    .use_interrupt = 0,
    .watchdog_cnt = 500,
    .set_defaults_hook = NULL,
    .pre_state_change_hook = NULL,
    .post_state_change_hook = NULL,
    .application_hook = NULL,
    .safeoutput_override = NULL,
    .pre_object_download_hook = NULL,
    .post_object_download_hook = NULL,
    .rxpdo_override = NULL,
    .txpdo_override = NULL,
    .esc_hw_interrupt_enable = NULL,
    .esc_hw_interrupt_disable = NULL,
    .esc_hw_eep_handler = NULL,
    .esc_check_dc_handler = NULL,
};


void esc_pdi_debug()
{
    // Read few core CSR registers to verify PDI is working
    uint8_t value = 0;
    printf("\r\n[ESC debug] ");
        
    ESC_read(0x0000, (void*) &value, sizeof(uint8_t));
    printf("Type [0x0000]: %s, ", STR5(value));
    ESC_read(0x0001, (void*) &value, sizeof(uint8_t));
    printf("Revision [0x0001]: %s, ", STR5(value));
    ESC_read(0x0004, (void*) &value, sizeof(uint8_t));
    printf("FMMU count [0x0004]: %s, ", STR5(value));
    ESC_read(0x0005, (void*) &value, sizeof(uint8_t));
    printf("Sync Managers count [0x0005]: %s, ", STR5(value));
    ESC_read(0x0006, (void*) &value, sizeof(uint8_t));
    printf("RAM size [0x0006]: %s, \r\n", STR5(value));
    
    printf("\r\n");
}


//---- user application ------------------------------------------------------------------------------

void cb_get_inputs()
{
    // write to slave TxPDO  
    // dummy value, so that sawtooth value profile will be seen constantly changing by ecat master 
    Obj.Counter++;
}

void cb_set_outputs()
{
    // read slave RxPDO  
    if (Obj.LedIn)
	{
		STM_EVAL_LEDOn(outPins[0]);
	}
	else 
	{
		STM_EVAL_LEDOff(outPins[0]);
	}
}


int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	APP_USART_Init();
	delay_init(); 
	printf("\r\n[ESC Setup] %s \r\n", "Started");
  	// setup LED pins
    STM_EVAL_LEDInit(outPins[0]);
    ecat_slv_init(&config);
	printf("\r\n[ESC Setup] Done, ready \r\n\n");
    esc_pdi_debug();

	while (1)
	{
		ecat_slv();

		if (ESCvar.ALerror) {
			printf("\rAL Error %d\r\n", ESCvar.ALerror);
		}
	}
}
