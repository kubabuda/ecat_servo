#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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


const uint8_t USER_BUTTON_PIN = 9;



unsigned long previousMs = 0;

const int OUTPUTS_COUNT = 8;

const int outPins[8] = {
  BitOut0, BitOut1, BitOut2, BitOut3, BitOut4, BitOut5, BitOut6, BitOut7
};


//---- setup ---------------------------------------------------------------------------------------
 
void setup()
{
  Serial.begin(9600);
  Serial.print("Initialization...");
  ecat_slv_init(&config);

  for (size_t i = 0; i < OUTPUTS_COUNT; i++)
  {
    int outPin = outPins[i];
    pinMode(outPin, OUTPUT);
  }


}


//---- main loop ----------------------------------------------------------------------------------------

void loop()
{ 
  ecat_slv();

  if (ESCvar.ALerror) {
    printf("\rAL Error %d", ESCvar.ALerror);
  }
}


//---- user application ------------------------------------------------------------------------------

void ecatapp()
{
  static uint32_t next_status_print_ms = 200;

  if ((digitalRead(USER_BUTTON_PIN) == LOW) & (millis() > next_status_print_ms))
  {
    Serial.write("\r\nPosition %lu\r\n", Obj.Target_position);
    next_status_print_ms = millis() + 200;
  }
}


void cb_get_inputs()
{

}

void cb_set_outputs()
{

}
