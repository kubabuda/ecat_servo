#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "cia402.h"

#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"

#ifdef __cplusplus
}
#endif // __cplusplus


/* CANopen Object Dictionary */
_Objects    Obj;


/* Application hook declaration */
void ecatapp();


/* SOES configuration */
static esc_cfg_t config = { 
    .user_arg = NULL,
    .use_interrupt = 0,
    .watchdog_cnt = 500,
    .skip_default_initialization = false,
    .set_defaults_hook = NULL,
    .pre_state_change_hook = NULL,
    .post_state_change_hook = NULL,
    .application_hook = ecatapp,
    .safeoutput_override = NULL,
    .pre_object_download_hook = NULL,
    .post_object_download_hook = NULL,
    .pre_object_upload_hook = NULL,
    .post_object_upload_hook = NULL,
    .rxpdo_override = NULL,
    .txpdo_override = NULL,
    .esc_hw_interrupt_enable = NULL,
    .esc_hw_interrupt_disable = NULL,
    .esc_hw_eep_handler = NULL,
    .esc_check_dc_handler = NULL,
};


/* CiA402 motion control hooks declaration */
void app_cia402_init(void);
void app_cia402_mc(void);


/* CiA402 motion control configuration */
cia402_axis_t cia402axis = {
    .state = NOT_READY_TO_SWITCH_ON,
    .statusword = &Obj.Status_Word,
    .ALstatus = &ESCvar.ALstatus,
    .init_od_hook = app_cia402_init,
    .motion_control_hook = app_cia402_mc,
};


const uint8_t USER_BUTTON_PIN = PB9;

void setup() {
  pinMode(USER_BUTTON_PIN, INPUT_PULLUP);
  Serial1.begin(SERIAL_SPEED);
  Serial1.print("\r\nESC setup started");

  ecat_slv_init(&config);
  Serial1.print("\r\nESC setup done");
  
  cia402_init(&cia402axis);
  Serial1.print("\r\nCiA402 servo setup done, ready\r\n");
}

void loop() {
  ecat_slv();

  if (ESCvar.ALerror) {
    Serial1.print("\rAL Error\r\n"); //%d", ESCvar.ALerror);
  }
}

//---- user application ------------------------------------------------------------------------------

void ecatapp()
{
  cia402_state_machine(&cia402axis, Obj.Control_Word);

  static uint32_t next_status_print_ms = 200;

  if ((digitalRead(USER_BUTTON_PIN) == LOW) & (millis() > next_status_print_ms))
  {
    //Serial1.print("\r\nPosition %lu", Obj.Target_position);
    next_status_print_ms = millis() + 200;
  }
}


void cb_get_inputs()
{

}

void cb_set_outputs()
{

}

void app_cia402_init(void)
{
    cia402axis.ALstatus = &ESCvar.ALstatus;
    cia402axis.statusword = &Obj.Status_Word;
}

void app_cia402_mc()
{
    // TODO motion control here
    Obj.Position_actual = Obj.Target_position; // dummy loopback
    // csp is the only supported mode
    *(cia402axis.statusword) |= CIA402_STATUSWORD_CSP_DRIVE_FOLLOWS_COMMAND;
}
 