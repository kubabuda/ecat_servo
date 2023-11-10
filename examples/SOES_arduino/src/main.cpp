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

const uint8_t USER_BUTTON_PIN = PB9;

void esc_pdi_debug()
{
    // Read few core CSR registers to verify PDI is working
    uint8_t type0x0000        = 0;
    uint8_t rev0x0001         = 0;
    uint8_t syncmgrscnt0x0005 = 0;
    uint8_t ramsize0x0006     = 0;    
                                                    // Expected for: LAN9252
    ESC_read(0x0000, (void*) &type0x0000,        sizeof(uint8_t)); // 0xC0=192  
    ESC_read(0x0001, (void*) &rev0x0001,         sizeof(uint8_t)); //  2
    ESC_read(0x0005, (void*) &syncmgrscnt0x0005, sizeof(uint8_t)); //  4
    ESC_read(0x0006, (void*) &ramsize0x0006,     sizeof(uint8_t)); //  4 [KB]

    printf("\r\n[ESC debug] Type [0x0000]: %#X, Revision [0x0001]: %#X, FMMU count [0x0005]: %#X, RAM size [0x0006]: %#X \r\n", 
        type0x0000, rev0x0001, syncmgrscnt0x0005, ramsize0x0006);
}


void setup() {
  pinMode(USER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(SERIAL_SPEED);
  printf("\r\nESC setup started");

  ecat_slv_init(&config);
  printf("\r\nESC setup done");
  digitalWrite(LED_BUILTIN, HIGH);
  esc_pdi_debug();
}

void loop() {
  ecat_slv();

  if (ESCvar.ALerror) {
    printf("\rAL Error\r\n"); //%d", ESCvar.ALerror);
  }
}

//---- user application ------------------------------------------------------------------------------


void ecatapp()
{
  static uint32_t next_status_print_ms = 200;

  if ((digitalRead(USER_BUTTON_PIN) == LOW) & (millis() > next_status_print_ms))
  {
    printf("\r\nKey %s\r\n", "press");
    next_status_print_ms = millis() + 200;
  }
}


void cb_get_inputs()
{
  digitalWrite(LED_BUILTIN, Obj.LedIn ? LOW: HIGH);
}

void cb_set_outputs()
{
  Obj.Key1 = digitalRead(USER_BUTTON_PIN);
}
