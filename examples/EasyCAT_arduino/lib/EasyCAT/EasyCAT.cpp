
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

#include "EasyCAT.h"
#include "LAN9252spi.h"

  
PROCBUFFER_OUT EasyCAT_BufferOut;               // output process data buffer 
PROCBUFFER_IN EasyCAT_BufferIn;                 // input process data buffer    

static SyncMode Sync_;

//---- EasyCAT board initialization ---------------------------------------------------------------

bool EasyCAT_Init(unsigned char SPI_CHIP_SELECT, SyncMode Sync)
{
  #define SPI_TimeoutMs 1000

  Sync_ = Sync;

  ULONG TempLong;
  unsigned short i;

  SPISetup(SPI_CHIP_SELECT);
  
  SPIWriteRegisterDirect (RESET_CTL, DIGITAL_RST);        // LAN9252 reset 
   
  i = 0;                                                  // reset timeout 
  do                                                      // wait for reset to complete
  {                                                       //
    i++;                                                  //
    TempLong.Long = SPIReadRegisterDirect (RESET_CTL, 4); //
  }while (((TempLong.Byte[0] & 0x01) != 0x00) && (i != SPI_TimeoutMs));    
                                                          //                                                       
  if (i == SPI_TimeoutMs)                                          // time out expired      
  {                                                       //
    SPIEnd();
    return false;                                         // initialization failed  
  }
  
  i = 0;                                                  // reset timeout  
  do                                                      // check the Byte Order Test Register
  {                                                       //
    i++;                                                  //      
    TempLong.Long = SPIReadRegisterDirect (BYTE_TEST, 4); //
  }
  while ((TempLong.Long != 0x87654321) && (i != SPI_TimeoutMs));  //    
                                                          //                                                            
  if (i == SPI_TimeoutMs)                                          // time out expired      
  {                                                       //
    SPIEnd();
    return false;                                         // initialization failed  
  }            
  
  i = 0;                                                  // reset timeout  
  do                                                      // check the Ready flag
  {                                                       //
    i++;                                                  //    
    TempLong.Long = SPIReadRegisterDirect (HW_CFG, 4);    //
  }while (((TempLong.Byte[3] & READY) == 0) && (i != SPI_TimeoutMs));//
                                                          //
  if (i == SPI_TimeoutMs)                                          // time out expired      
  {                                                       //
    SPIEnd();
    return false;                                         // initialization failed  
  }            
                  
  if ((Sync_ == DC_SYNC) || (Sync_ == SM_SYNC))           //--- if requested, enable --------   
  {                                                       //--- interrupt generation -------- 
  
    if (Sync_ == DC_SYNC)
    {                                                     // enable interrupt from SYNC 0
      SPIWriteRegisterIndirect (0x00000004, AL_EVENT_MASK, 4);  
                                                          // in AL event mask register, and disable 
                                                          // other interrupt sources    
    }                                                       
                                                                                                         
    else
    {                                                     
                                                          // enable interrupt from SM 0 event 
                                                          // (output synchronization manager)
      SPIWriteRegisterIndirect (0x00000100, AL_EVENT_MASK, 4);     
                                                          // in AL event mask register, and disable 
                                                          // other interrupt sources 
    }    
                                                         
    SPIWriteRegisterDirect (IRQ_CFG, 0x00000111);         // set LAN9252 interrupt pin driver  
                                                          // as push-pull active high
                                                          // (On the EasyCAT shield board the IRQ pin
                                                          // is inverted by a mosfet, so Arduino                                                        
                                                          // receives an active low signal)
                                                                        
    SPIWriteRegisterDirect (INT_EN, 0x00000001);          // enable LAN9252 interrupt      
  } 
  
  // TempLong.Long = SPIReadRegisterDirect (ID_REV, 4);
  // short detectedChip = TempLong.Word[1];
  // short revision = TempLong.Word[0];

  return true;                                        // initalization completed   
}  


//---- EtherCAT task ------------------------------------------------------------------------------

unsigned char EasyCAT_MainTask()                    // must be called cyclically by the application
{
  bool WatchDog = 0;
  bool Operational = 0; 
  unsigned char i;
  ULONG TempLong; 
  unsigned char Status;  
 
  TempLong.Long = SPIReadRegisterIndirect (WDOG_STATUS, 1); // read watchdog status
  if ((TempLong.Byte[0] & 0x01) == 0x01)                    //
    WatchDog = 0;                                           // set/reset the corrisponding flag
  else                                                      //
    WatchDog = 1;                                           //
    
  TempLong.Long = SPIReadRegisterIndirect (AL_STATUS, 1);   // read the EtherCAT State Machine status
  Status = TempLong.Byte[0] & 0x0F;                         // to see if we are in operational state
  if (Status == ESM_OP)                                     // 
    Operational = 1;                                        //
  else                                                      // set/reset the corrisponding flag
    Operational = 0;                                        //    


                                                            //--- process data transfert ----------
                                                            //                                                        
  if (WatchDog | !Operational)                              // if watchdog is active or we are 
  {                                                         // not in operational state, reset 
    for (i=0; i < TOT_BYTE_NUM_OUT ; i++)                   // the output buffer
    {                                                       //
      EasyCAT_BufferOut.Byte[i] = 0;                                //
    }                                                       //
  }
  
  else                                                      
  {                                                         
    SPIReadProcRamFifo();                                   // otherwise transfer process data from 
  }                                                         // the EtherCAT core to the output buffer  
                 
  SPIWriteProcRamFifo();                                    // we always transfer process data from
                                                            // the input buffer to the EtherCAT core  

  if (WatchDog)                                             // return the status of the State Machine      
  {                                                         // and of the watchdog
    Status |= 0x80;                                         //
  }                                                         //
  return Status;                                            //     
}
