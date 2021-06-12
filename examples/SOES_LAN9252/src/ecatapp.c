#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"
#include "ecatapp.h"

#include "delay.h"
#include "usart.h"
#include "stm32f4_discovery.h"


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
    .application_hook = ecatapp,
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


void ecatapp_setup()
{
    // setup LED pins
    for (uint8_t i = 0; i < OUTPUT_BITS_COUNT; i++)
    {
        STM_EVAL_LEDInit(outPins[i]);
    }

    ecat_slv_init(&config);
    esc_pdi_debug();
}


uint32_t ecatapp_benchmark_us()
{ 
    STM_EVAL_PBInit(BUTTON_MODE_GPIO);
    // benchmark start
    static volatile uint32_t hiscore = 0;
    // delay_init();
    
    stopwatch_t st;
    stopwatch_start(&st);
    
    ecat_slv();
    
    // benchmark stop
    volatile uint32_t elapsed_us = stopwatch_now_us(&st);

    if (elapsed_us > hiscore) 
    {
        hiscore = elapsed_us;
    }

    printf("\r[ESC benchmark] %s us (%s top)", STR5(elapsed_us), STR5(hiscore));

    if (STM_EVAL_IsPressed())
    {
        hiscore = 0;
    }

    return elapsed_us;
}

//---- user application ------------------------------------------------------------------------------

volatile uint8_t actual_value;

void ecatapp() 
{
    actual_value = Obj.CounterOut + 1;
    
    if((Obj.Control_Word != 0) & (config.rxpdo_override == NULL))
    {
        // first nonzero controlword turns on PDO override
        
    }
    else if ((Obj.Control_Word == 0) & (config.rxpdo_override != NULL))
    {
        // zero on controlword returns default PDO handling
        
    }    
}


void cb_get_inputs()
{
    // write to slave TxPDO  
    // dummy value, so that sawtooth value profile will be seen constantly changing by ecat master 
    Obj.CounterOut = actual_value;
}

void cb_set_outputs()
{
    // read slave RxPDO  
    for (uint8_t i = 0; i < OUTPUT_BITS_COUNT; i++)
    {
        if (Obj.LedIn & (1 << i))
        {
            STM_EVAL_LEDOn(outPins[i]);
        }
        else 
        {
            STM_EVAL_LEDOff(outPins[i]);
        }
    }
}

void ecatapp_mainloop ()    
{
    /* --------- Slave Stack Code communication --------- */
    ecatapp_benchmark_us();
}
