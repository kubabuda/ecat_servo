#include "ECAT_App.h"
#include "EasyCAT.h"
#include "delay.h"
#include "stm32f4_discovery.h"


static uint32_t previousMs = 0;


void ECAT_AppSetup()
{
    // setup LED pins
    for (uint8_t i = 0; i < OUTPUT_BITS_COUNT; i++)
    {
        STM_EVAL_LEDInit(outPins[i]);
    }

    if (EasyCAT_Init(ASYNC) != true)
    {
        const Led_TypeDef errorLed = LED6;
        // initialization failed, blink LED until reset  
        STM_EVAL_LEDInit(errorLed);
        
        while (1)
        {
            STM_EVAL_LEDToggle(errorLed);
            delay_ms(1000);
        }
    }
    previousMs = millis();
}


uint32_t ECAT_PDIcommBenchmark_us()
{ 
    // benchmark start
    uint32_t start = micros();

    // The EasyCAT cycle and the Master cycle are asynchronous
    EasyCAT_MainTask();
    // ECAT_Application();
    
    // benchmark stop
    uint32_t stop = micros();
    uint32_t elapsedUs = stop - start;

    return elapsedUs;
}


//---- user application ------------------------------------------------------------------------------

#define ECAT_APP_CYCLE_TIME_MS 10

void ECAT_Application ()                                        
{
    EasyCAT_MainTask();

    unsigned long nowMs = millis();

    if (nowMs - previousMs >= ECAT_APP_CYCLE_TIME_MS)
    {      
        previousMs = nowMs;

        // read slave RxPDO  
        for (uint8_t i = 0; i < OUTPUT_BITS_COUNT; i++)
        {  
            if (EasyCAT_BufferOut.Cust.Position_0x6064 & (1 << i))
            {
                STM_EVAL_LEDOn(outPins[i]);
            }
            else {
                STM_EVAL_LEDOff(outPins[i]);
            }
        }

        // write to slave TxPDO  
        
        // dummy value, so that value will be seen constantly changing by ecat master 
        uint32_t actualPosition = EasyCAT_BufferIn.Cust.ActualPosition_0x6064 + 1;
        EasyCAT_BufferIn.Cust.ActualPosition_0x6064 = actualPosition;
    }
}