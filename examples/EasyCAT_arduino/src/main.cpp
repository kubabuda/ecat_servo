#include "main.h"
//********************************************************************************************
//                                                                                           *
// AB&T Tecnologie Informatiche - Ivrea Italy                                                *
// http://www.bausano.net                                                                    *
// https://www.ethercat.org/en/products/791FFAA126AD43859920EA64384AD4FD.htm                 *
//                                                                                           *  
//********************************************************************************************    

//********************************************************************************************    
//                                                                                           *
// This software is distributed as an example, "AS IS", in the hope that it could            *
// be useful, WITHOUT ANY WARRANTY of any kind, express or implied, included, but            *
// not limited,  to the warranties of merchantability, fitness for a particular              *
// purpose, and non infringiment. In no event shall the authors be liable for any            *    
// claim, damages or other liability, arising from, or in connection with this software.     *
//                                                                                           *
//******************************************************************************************** 

#include "main.h"
#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252


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

  for (size_t i = 0; i < OUTPUTS_COUNT; i++)
  {
    int outPin = outPins[i];
    pinMode(outPin, OUTPUT);
  }

  if (EasyCAT_Init(SpiCS_Pin, ASYNC) == true) {
    Serial.println("done.");
  }
  else 
  {
    // initialization failed, blink LED until reset
    Serial.println("failed!");
    pinMode(BitOut0, OUTPUT);

    while(1)
    {
      digitalWrite (BitOut0, LOW);
      delay(500);
      digitalWrite (BitOut0, HIGH);
      delay(500);
    }
  }  
  previousMs = millis();
}


//---- main loop ----------------------------------------------------------------------------------------

void loop()
{ 
  // unsigned long zero = micros(); // benchmark start
  // unsigned long start = micros();
  
  // The EasyCAT cycle and the Master cycle are asynchronous
  EasyCAT_MainTask();

  // unsigned long end = micros(); // benchmark stop
  // static volatile unsigned long cycleTimeUs = end - start - (start - zero);
  // Serial.print("Comm cycle time [us]: ");
  // Serial.println(cycleTimeUs);

  Application();
}


//---- user application ------------------------------------------------------------------------------

void Application ()                                        

{
  #define ECAT_APP_CYCLE_TIME_MS 10

  unsigned long nowMs = millis();
  if (nowMs - previousMs >= ECAT_APP_CYCLE_TIME_MS)
  {      
    previousMs = nowMs;
    
    // read slave RxPDO  
    for (size_t i = 0; i < OUTPUTS_COUNT; i++)
    {
      int outPin = outPins[i];
      
      if (EasyCAT_BufferOut.Cust.Position_0x6064 & (1 << i)) {
        digitalWrite (outPin, HIGH);
      }
      else {
        digitalWrite (outPin, LOW);
      }
    }

    // write to slave TxPDO  
    // dummy value, so that value will be seen constantly changing by ecat master 
    uint32_t actualPosition = EasyCAT_BufferIn.Cust.ActualPosition_0x6064 + 1;
    EasyCAT_BufferIn.Cust.ActualPosition_0x6064 = actualPosition;
  }
}
