
#ifndef LAN9252_SPI_H
#define LAN9252_SPI_H

#include <stdint.h>

//---- LAN9252 registers --------------------------------------------------------------------------

                                            //---- access to EtherCAT registers -------------------

#define ECAT_CSR_DATA           0x0300      // EtherCAT CSR Interface Data Register
#define ECAT_CSR_CMD            0x0304      // EtherCAT CSR Interface Command Register

//---- LAN9252 flags ------------------------------------------------------------------------------

#define ECAT_CSR_BUSY     0x80
#define PRAM_ABORT        0x40000000
#define PRAM_BUSY         0x80
#define PRAM_AVAIL        0x01
#define READY             0x08
#define DIGITAL_RST       0x00000001

//---- LAN9252 SPI commands -----------------------------------------------------------------------

#define COMM_SPI_READ    0x03
#define COMM_SPI_WRITE   0x02

#define DUMMY_BYTE       0xFF

//---- STM32F4 SPI settings -----------------------------------------------------------------------

#if defined (STM32F4)  

  #define SPIX_ESC                        SPI1 
  #define ESC_RCC_APB2PERIPH_SPIX         RCC_APB2Periph_SPI1
  #define ESC_GPIOX_AF_SPIx               GPIO_AF_SPI1


  #define ESC_RCC_APB1PERIPH_GPIOX_CTRL   RCC_AHB1Periph_GPIOA
  #define ESC_GPIOX_CTRL                  GPIOA
  
  #define ESC_GPIO_Pin_SCK                GPIO_Pin_5
  #define ESC_GPIO_PinSourceSCK           5

  #define ESC_GPIO_Pin_MISO               GPIO_Pin_6
  #define ESC_GPIO_PinSource_MISO         6

  #define ESC_GPIO_Pin_MOSI               GPIO_Pin_7
  #define ESC_GPIO_PinSource_MOSI         7

  #define ESC_RCC_APB1PERIPH_GPIOX_CS     RCC_AHB1Periph_GPIOA
  #define ESC_GPIOX_CS                    GPIOA
  
  #define ESC_GPIO_Pin_CS                 GPIO_Pin_4


#else
  #error "Architecture not supported"
#endif


void SPISetup();
void SPIEnd();

void SPIWriteRegisterDirect(unsigned short address, unsigned long dataOut);
unsigned long SPIReadRegisterDirect(unsigned short address, unsigned char Len);

void SPIWriteRegisterIndirect(unsigned long sataOut, unsigned short address, unsigned char Len);
unsigned long SPIReadRegisterIndirect(unsigned short address, unsigned char Len); 

void SPIReadProcRamFifo();
void SPIWriteProcRamFifo();


#endif // LAN9252_SPI_H
