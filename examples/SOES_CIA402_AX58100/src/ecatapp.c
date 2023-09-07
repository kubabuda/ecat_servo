#include "usart.h"

#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"

#include "esc_irq.h"
#include "pdo_override.h"
#include "cia402device.h"
#include "ecatapp.h"


/* CANopen Object Dictionary */
_Objects    Obj;


/* Application hook declaration */
void ecatapp(void);
uint16_t check_dc_handler(void);

/* CiA402 hooks declarations */
void app_cia402_init(void);
void app_cia402_mc(void);


/* SOES configuration */
static esc_cfg_t config = { 
    .user_arg                  = "ax58100",
    .use_interrupt             = 1,
    .watchdog_cnt              = INT32_MAX,
    .set_defaults_hook         = NULL,
    .pre_state_change_hook     = NULL,
    .post_state_change_hook    = NULL,
    .application_hook          = ecatapp,
    .safeoutput_override       = NULL,
    .pre_object_download_hook  = NULL,
    .post_object_download_hook = NULL,
    .rxpdo_override            = rxpdo_override,
    .txpdo_override            = txpdo_override,
    .esc_hw_interrupt_enable   = ESC_interrupt_enable,
    .esc_hw_interrupt_disable  = ESC_interrupt_disable,
    .esc_hw_eep_handler        = NULL,
    .esc_check_dc_handler      = check_dc_handler,
};

/* CiA402 motion control configuration */
cia402_axis_t cia402axis = {
    .init_od_hook              = app_cia402_init,
    .motion_control_hook       = app_cia402_mc,
};

// **************************************************************

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
// **************************************************************

void ecatapp_init(void) {
    ecat_slv_init(&config);
    cia402_init(&cia402axis);
	init_override();
}

uint16_t check_dc_handler (void)
{
    // minimum watchdog value is 1 ms, in nanoseconds
    #define MIN_WATCHDOG_VALUE_NS      1000000

    /* Indicate we run DC */
    ESCvar.dcsync = 1;
    /* Fetch the sync counter limit (SDO10F1) */
    ESCvar.synccounterlimit = Obj.Error_Settings.SyncErrorCounterLimit;

    uint32_t sync0cycleTime = ESC_enable_DC();
    Obj.Sync_Manager_2_Parameters.CycleTime = sync0cycleTime;
    // Obj.Sync_Manager_3_Parameters.CycleTime = sync0cycleTime;
    // calculate watchdog value as 2 x SYNC0 cycle time
    int watchdog_value = 2 * sync0cycleTime;
    if (watchdog_value < MIN_WATCHDOG_VALUE_NS) {
        watchdog_value = MIN_WATCHDOG_VALUE_NS;
    }
    APP_setwatchdog(watchdog_value);

    return 0;
}

void ecatapp()
{
    cia402_state_machine(&cia402axis, Obj.Control_Word);
}


void cb_get_inputs()
{
    /* SOES reqires this function but nothing to do here in CiA402 app */
}


void cb_set_outputs()
{
    /* SOES reqires this function but nothing to do here in CiA402 app */
}


void app_cia402_init(void)
{
    /* Match CiA 402 objects to used CoE Object Dictionary implementation */
    cia402axis.statusword = &Obj.Status_Word;
    cia402axis.ALstatus = &ESCvar.ALstatus;
}


void app_cia402_mc()
{
    // TODO motion control here
    Obj.Position_actual = Obj.Target_position; // dummy loopback
    // csp is the only supported mode for now
    *(cia402axis.statusword) |= CIA402_STATUSWORD_CSP_DRIVE_FOLLOWS_COMMAND;
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
