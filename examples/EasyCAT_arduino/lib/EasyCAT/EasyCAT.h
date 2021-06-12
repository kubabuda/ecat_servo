
#ifndef ABT_EasyCAT_H
#define ABT_EasyCAT_H

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



//---- EasyCAT library V_2_0 ------------------------------------------------------------	


// This library has been tested with Arduino IDE 1.8.5
// https://www.arduino.cc
 
 

//--- V_2.0 -----
//
// All the code moved in the .h file:
// this seems to be the only way to allow the library configuration
// through "#defines" in the ino. file
//
// New customization capability through the Easy Configurator tool 
// Fixed SPI optimization for SAMD


//--- V_1.5 ---
//
// Added support for 16+16, 32+32, 64+64 and 128+128 I/O bytes, through definition of 
// BYTE_NUM constant, in "EasyCAT.h"
// Added support for any number of bytes transferred, between 0 and 128, through definition 
// of CUST_BYTE_NUM_IN and CUST_BYTE_NUM_OUT constants, in "EasyCAT.h"
// Added support for DC and SM synchronization
// Aborted any possible pending transfer before accessing the process ram fifos
// Cleared all output bytes correctly, in the event of watchdog
// Fixed chip reset procedure
// Added parameter "Len" in function "SPIWriteRegisterIndirect"
// At init, print out chip identification, revision and number of bytes transferred


//--- V_1.4 ---
//
// The MainTask function now return the state of the 
// Ethercat State machine and of the Wachdog
// Now the SPI chip select is initialized High inside the constructor


//--- V_1.3 --- 
// Replaced delay(100) in Init() function with a wait loop 


//--- V_1.2 --- 
// SPI chip select is configured by the application, setting a constructor parameter.
// If no chip select is declared in the constructor, pin 9 will be used as default.
// Code cleaning.
// Comments in english.
// TODO: fast SPI transfert for DUE board


//--- V_1.1 --- 
// First official release.
// SPI chip select is configured editing the library ".h" file.


#include <stdint.h>
#include "DS402_PDOs_EasyCatProject.h"
 
 
#define TOT_BYTE_NUM_OUT  CUST_BYTE_NUM_OUT       // any number between 0 and 128  


#if TOT_BYTE_NUM_OUT > 64                           // if we have more then  64 bytes
                                                    // we have to split the transfer in two
                                                        
  #define SEC_BYTE_NUM_OUT  (TOT_BYTE_NUM_OUT - 64) // number of bytes of the second transfer
                                        
  #if ((SEC_BYTE_NUM_OUT & 0x03) != 0x00)           // number of bytes of the second transfer
                                                    // rounded to long
    #define SEC_BYTE_NUM_ROUND_OUT  ((SEC_BYTE_NUM_OUT | 0x03) + 1)  
  #else                                             // 
    #define SEC_BYTE_NUM_ROUND_OUT  SEC_BYTE_NUM_OUT//
  #endif                                            //

  #define FST_BYTE_NUM_OUT  64                      // number of bytes of the first transfer     
  #define FST_BYTE_NUM_ROUND_OUT  64                // number of bytes of the first transfer
                                                    // rounded to 4 (long)

#else                                               // if we have max 64 bytes we transfer
                                                    // them in just one round
                                                        
  #define FST_BYTE_NUM_OUT  TOT_BYTE_NUM_OUT        // number of bytes of the first and only transfer 
  
  #if ((FST_BYTE_NUM_OUT & 0x03) != 0x00)           // number of bytes of the first and only transfer  
                                                    // rounded to 4 (long)   
    #define FST_BYTE_NUM_ROUND_OUT ((FST_BYTE_NUM_OUT | 0x03) + 1)
  #else                                             //
    #define FST_BYTE_NUM_ROUND_OUT  FST_BYTE_NUM_OUT//   
  #endif                                            //

  #define SEC_BYTE_NUM_OUT  0                       // we don't use the second round
  #define SEC_BYTE_NUM_ROUND_OUT  0                 //
  
#endif


#define TOT_BYTE_NUM_IN  CUST_BYTE_NUM_IN         // any number between 0 and 128  


#if TOT_BYTE_NUM_IN > 64                            // if we have more then  64 bytes
                                                    // we have to split the transfer in two
                                                        
  #define SEC_BYTE_NUM_IN  (TOT_BYTE_NUM_IN - 64)   // number of bytes of the second transfer
 
  #if ((SEC_BYTE_NUM_IN & 0x03) != 0x00)            // number of bytes of the second transfer 
                                                    // rounded to 4 (long)          
    #define SEC_BYTE_NUM_ROUND_IN  ((SEC_BYTE_NUM_IN | 0x03) + 1)  
  #else                                             //
    #define SEC_BYTE_NUM_ROUND_IN  SEC_BYTE_NUM_IN  //
  #endif                                            //

  #define FST_BYTE_NUM_IN  64                       // number of bytes of the first transfer     
  #define FST_BYTE_NUM_ROUND_IN  64                 // number of bytes of the first transfer
                                                    // rounded to 4 (long)

#else                                               // if we have max 64 bytes we transfer
                                                    // them in just one round
                                                        
  #define FST_BYTE_NUM_IN  TOT_BYTE_NUM_IN          // number of bytes of the first and only transfer  

  #if ((FST_BYTE_NUM_IN & 0x03) != 0x00)            // number of bytes of the first and only transfer
                                                    // rounded to 4 (long)
    #define FST_BYTE_NUM_ROUND_IN ((FST_BYTE_NUM_IN | 0x03) + 1)
  #else                                             //
    #define FST_BYTE_NUM_ROUND_IN  FST_BYTE_NUM_IN  // 
  #endif                                            //

  #define SEC_BYTE_NUM_IN  0                        // we don't use the second round
  #define SEC_BYTE_NUM_ROUND_IN  0                  //

#endif
 
//---------------------------------------------------------------------------------

//----------------- sanity check -------------------------------------------------------                                 

#ifdef BYTE_NUM                     // STANDARD MODE and CUSTOM MODE
                                    // cannot be defined at the same time
  #ifdef CUST_BYTE_NUM_OUT 
    #error "BYTE_NUM and CUST_BYTE_NUM_OUT cannot be defined at the same time !!!!"
    #error "define them correctly in file EasyCAT.h"
    #endif
  
  #ifdef CUST_BYTE_NUM_IN 
    #error "BYTE_NUM and CUST_BYTE_NUM_IN cannot be defined at the same time !!!!"
    #error "define them correctly in file EasyCAT.h"
  #endif
#endif 
  
//*************************************************************************************************

//---- LAN9252 registers --------------------------------------------------------------------------

                                            //---- access to EtherCAT registers -------------------

#define ECAT_CSR_DATA           0x0300      // EtherCAT CSR Interface Data Register
#define ECAT_CSR_CMD            0x0304      // EtherCAT CSR Interface Command Register


                                            //---- access to EtherCAT process RAM ----------------- 

#define ECAT_PRAM_RD_ADDR_LEN   0x0308      // EtherCAT Process RAM Read Address and Length Register
#define ECAT_PRAM_RD_CMD        0x030C      // EtherCAT Process RAM Read Command Register
#define ECAT_PRAM_WR_ADDR_LEN   0x0310      // EtherCAT Process RAM Write Address and Length Register 
#define ECAT_PRAM_WR_CMD        0x0314      // EtherCAT Process RAM Write Command Register

#define ECAT_PRAM_RD_DATA       0x0000      // EtherCAT Process RAM Read Data FIFO
#define ECAT_PRAM_WR_DATA       0x0020      // EtherCAT Process RAM Write Data FIFO

                                            //---- EtherCAT registers -----------------------------

#define AL_CONTROL              0x0120      // AL control                                             
#define AL_STATUS               0x0130      // AL status
#define AL_STATUS_CODE          0x0134      // AL status code
#define AL_EVENT                0x0220      // AL event request
#define AL_EVENT_MASK           0x0204      // AL event interrupt mask

#define WDOG_STATUS             0x0440      // watch dog status

#define SM0_BASE                0x0800      // SM0 base address (output)
#define SM1_BASE                0x0808      // SM1 base address (input) 


                                            //---- LAN9252 registers ------------------------------    

#define HW_CFG                  0x0074      // hardware configuration register
#define BYTE_TEST               0x0064      // byte order test register
#define RESET_CTL               0x01F8      // reset register       
#define ID_REV                  0x0050      // chip ID and revision
#define IRQ_CFG                 0x0054      // interrupt configuration
#define INT_EN                  0x005C      // interrupt enable


//---- LAN9252 flags ------------------------------------------------------------------------------

#define ECAT_CSR_BUSY     0x80
#define PRAM_ABORT        0x40000000
#define PRAM_BUSY         0x80
#define PRAM_AVAIL        0x01
#define READY             0x08
#define DIGITAL_RST       0x00000001


//---- EtherCAT flags -----------------------------------------------------------------------------

#define ALEVENT_CONTROL         0x0001
#define ALEVENT_SM              0x0010
 
 
//----- state machine ------------------------------------------------------------

#define ESM_INIT                  0x01          // state machine control
#define ESM_PREOP                 0x02          // (state request)
#define ESM_BOOT                  0x03          // 
#define ESM_SAFEOP                0x04          // safe-operational
#define ESM_OP                    0x08          // operational
    
    
//--- ESC commands --------------------------------------------------------------------------------

#define ESC_WRITE 		   0x80
#define ESC_READ 		     0xC0


//---- SPI ----------------------------------------------------------------------------------------

#define COMM_SPI_READ    0x03
#define COMM_SPI_WRITE   0x02

#define DUMMY_BYTE       0xFF


//---- typedef ------------------------------------------------------------------------------------

typedef union
{
    unsigned short  Word;
    unsigned char   Byte[2];
} UWORD;

typedef union
{
    unsigned long   Long;
    unsigned short  Word[2];
    unsigned char   Byte[4];
} ULONG;


typedef enum  
{
  ASYNC,
  DC_SYNC,
  SM_SYNC
} SyncMode;

//-------------------------------------------------------------------------------------------------
 
extern PROCBUFFER_OUT EasyCAT_BufferOut;               // output process data buffer 
extern PROCBUFFER_IN EasyCAT_BufferIn;                 // input process data buffer    

                                                       // EasyCAT board initialization 
bool EasyCAT_Init(unsigned char SPI_CHIP_SELECT, SyncMode Sync);                            
unsigned char EasyCAT_MainTask();                      // EtherCAT main task
                                                       // must be called cyclically by the application 

//---------------------------------------------------------------------------------------- 

#endif // ABT_EasyCAT_H
