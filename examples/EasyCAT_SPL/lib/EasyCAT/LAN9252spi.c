#include "LAN9252spi.h"
#include "EasyCAT.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"


#define SCS_Low_macro      ESC_GPIOX_CS->BSRRH = GPIO_Pin_4;
#define SCS_High_macro     ESC_GPIOX_CS->BSRRL = GPIO_Pin_4;


void SPISetup(void) 
{ 	
	GPIO_InitTypeDef gpio;
  SPI_InitTypeDef  spi;
  
	RCC_APB2PeriphClockCmd(ESC_RCC_APB2PERIPH_SPIX, ENABLE);
  RCC_AHB1PeriphClockCmd(ESC_RCC_APB1PERIPH_GPIOX_CTRL, ENABLE);
  RCC_AHB1PeriphClockCmd(ESC_RCC_APB1PERIPH_GPIOX_CS, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
	GPIO_PinAFConfig(ESC_GPIOX_CTRL, ESC_GPIO_PinSourceSCK,  ESC_GPIOX_AF_SPIx);
	GPIO_PinAFConfig(ESC_GPIOX_CTRL, ESC_GPIO_PinSource_MISO, ESC_GPIOX_AF_SPIx);
	GPIO_PinAFConfig(ESC_GPIOX_CTRL, ESC_GPIO_PinSource_MOSI, ESC_GPIOX_AF_SPIx);

  /* SPI SCK pin configuration */
  gpio.GPIO_Pin   = ESC_GPIO_Pin_SCK;
  gpio.GPIO_Mode  = GPIO_Mode_AF;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(ESC_GPIOX_CTRL, &gpio);
	
	/* SPI  MISO pin configuration */
  gpio.GPIO_Pin   = ESC_GPIO_Pin_MISO;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(ESC_GPIOX_CTRL, &gpio);  

  /* SPI  MOSI pin configuration */
  gpio.GPIO_Pin   = ESC_GPIO_Pin_MOSI;
  gpio.GPIO_Mode  = GPIO_Mode_AF;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(ESC_GPIOX_CTRL, &gpio);
 
  /* CS */
  gpio.GPIO_Pin   = ESC_GPIO_Pin_CS; 
  gpio.GPIO_Mode  = GPIO_Mode_OUT;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ESC_GPIOX_CS, &gpio);
	
	SPI_I2S_DeInit(SPIX_ESC);
  spi.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  spi.SPI_DataSize          = SPI_DataSize_8b;
  spi.SPI_CPOL              = SPI_CPOL_Low;
  spi.SPI_CPHA              = SPI_CPHA_1Edge;
  spi.SPI_NSS               = SPI_NSS_Soft;
  spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spi.SPI_FirstBit          = SPI_FirstBit_MSB;
  spi.SPI_CRCPolynomial     = 7;
	spi.SPI_Mode              = SPI_Mode_Master;
  SPI_Init(SPIX_ESC, &spi);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIX_ESC, ENABLE);
} 


inline static uint8_t SPI_Transfer(uint8_t byte)
{
    SPIX_ESC->DR = byte;
    while ((SPIX_ESC->SR & SPI_I2S_FLAG_RXNE) == RESET);
    return SPIX_ESC->DR;
}


//---- read a directly addressable registers  -----------------------------------------------------

unsigned long SPIReadRegisterDirect (unsigned short Address, unsigned char Len)

                                                   // Address = register to read
                                                   // Len = number of bytes to read (1,2,3,4)
                                                   //
                                                   // a long is returned but only the requested bytes
                                                   // are meaningful, starting from LsByte                                                 
{
  ULONG Result; 
  UWORD Addr;
  Addr.Word = Address; 
  unsigned char i; 
  
  SCS_Low_macro                                             // SPI chip select enable

  SPI_Transfer(COMM_SPI_READ);                            // SPI read command
  SPI_Transfer(Addr.Byte[1]);                             // address of the register
  SPI_Transfer(Addr.Byte[0]);                             // to read, MsByte first
 
  for (i=0; i<Len; i++)                                     // read the requested number of bytes
  {                                                         // LsByte first 
    Result.Byte[i] = SPI_Transfer(DUMMY_BYTE);            //
  }                                                         //    
  
  SCS_High_macro                                            // SPI chip select disable 
 
  return Result.Long;                                       // return the result
}


//---- write a directly addressable registers  ----------------------------------------------------

void SPIWriteRegisterDirect (unsigned short Address, unsigned long DataOut)
                                                   // Address = register to write
                                                   // DataOut = data to write
{ 
  ULONG Data; 
  UWORD Addr;
  Addr.Word = Address;
  Data.Long = DataOut;    

  
  SCS_Low_macro                                             // SPI chip select enable  
  
  SPI_Transfer(COMM_SPI_WRITE);                           // SPI write command
  SPI_Transfer(Addr.Byte[1]);                             // address of the register
  SPI_Transfer(Addr.Byte[0]);                             // to write MsByte first

  SPI_Transfer(Data.Byte[0]);                             // data to write 
  SPI_Transfer(Data.Byte[1]);                             // LsByte first
  SPI_Transfer(Data.Byte[2]);                             //
  SPI_Transfer(Data.Byte[3]);                             //
 
  SCS_High_macro                                            // SPI chip select enable   
}


//---- read an indirectly addressable registers  --------------------------------------------------

unsigned long SPIReadRegisterIndirect (unsigned short Address, unsigned char Len)
                                      // Address = register to read
                                      // Len = number of bytes to read (1,2,3,4)
                                      //
                                      // a long is returned but only the requested bytes
                                      // are meaningful, starting from LsByte                                                  
{
  ULONG TempLong;
  UWORD Addr;
  Addr.Word = Address;
                                                            // compose the command
                                                            //
  TempLong.Byte[0] = Addr.Byte[0];                          // address of the register
  TempLong.Byte[1] = Addr.Byte[1];                          // to read, LsByte first
  TempLong.Byte[2] = Len;                                   // number of bytes to read
  TempLong.Byte[3] = ESC_READ;                              // ESC read 

  SPIWriteRegisterDirect (ECAT_CSR_CMD, TempLong.Long);     // write the command

  do
  {                                                         // wait for command execution
    TempLong.Long = SPIReadRegisterDirect(ECAT_CSR_CMD,4);  //
  }                                                         //
  while(TempLong.Byte[3] & ECAT_CSR_BUSY);                  //
                                                             
                                                              
  TempLong.Long = SPIReadRegisterDirect(ECAT_CSR_DATA,Len); // read the requested register
  return TempLong.Long;                                     //
}


//---- write an indirectly addressable registers  -------------------------------------------------

void  SPIWriteRegisterIndirect (unsigned long DataOut, unsigned short Address, unsigned char Len)

                                // Address = register to write
                                // DataOut = data to write                                                    
{
  ULONG TempLong;
  UWORD Addr;
  Addr.Word = Address;

  
  SPIWriteRegisterDirect (ECAT_CSR_DATA, DataOut);            // write the data

                                                              // compose the command
                                                              //                                
  TempLong.Byte[0] = Addr.Byte[0];                            // address of the register  
  TempLong.Byte[1] = Addr.Byte[1];                            // to write, LsByte first
  TempLong.Byte[2] = Len;                                     // number of bytes to write
  TempLong.Byte[3] = ESC_WRITE;                               // ESC write

  SPIWriteRegisterDirect (ECAT_CSR_CMD, TempLong.Long);       // write the command

  do                                                          // wait for command execution
  {                                                           //
    TempLong.Long = SPIReadRegisterDirect (ECAT_CSR_CMD, 4);  //  
  }                                                           //  
  while (TempLong.Byte[3] & ECAT_CSR_BUSY);                   //
}


//---- read from process ram fifo ----------------------------------------------------------------

void SPIReadProcRamFifo()    // read data from the output process ram, through the fifo
  // these are the bytes received from the EtherCAT master and
  // that will be use by our application to write the outputs
{
  ULONG TempLong;
  unsigned char i;
  
  
  #if TOT_BYTE_NUM_OUT > 0

    SPIWriteRegisterDirect (ECAT_PRAM_RD_CMD, PRAM_ABORT);        // abort any possible pending transfer

  
    SPIWriteRegisterDirect (ECAT_PRAM_RD_ADDR_LEN, (0x00001000 | (((uint32_t)TOT_BYTE_NUM_OUT) << 16)));   
                                                                  // the high word is the num of bytes
                                                                  // to read 0xTOT_BYTE_NUM_OUT----
                                                                  // the low word is the output process        
                                                                  // ram offset 0x----1000 

    SPIWriteRegisterDirect (ECAT_PRAM_RD_CMD, 0x80000000);        // start command        
 
 
                                                //------- one round is enough if we have ----
                                                //------- to transfer up to 64 bytes --------
   
    do                                                            // wait for the data to be       
    {                                                             // transferred from the output  
      TempLong.Long = SPIReadRegisterDirect (ECAT_PRAM_RD_CMD,2); // process ram to the read fifo       
    }                                                             //    
    while (TempLong.Byte[1] != (FST_BYTE_NUM_ROUND_OUT/4));       // *CCC* 
  
    SCS_Low_macro                                                 // enable SPI chip select 
  
    SPI_Transfer(COMM_SPI_READ);                                // SPI read command
    SPI_Transfer(0x00);                                         // address of the read  
    SPI_Transfer(0x00);                                     // fifo MsByte first
  
    for (i=0; i< FST_BYTE_NUM_ROUND_OUT; i++)                     // transfer the data
    {                                                             //
      EasyCAT_BufferOut.Byte[i] = SPI_Transfer(DUMMY_BYTE);             //
    }                                                             //
    
    SCS_High_macro                                                // disable SPI chip select    
  #endif  

  
  #if SEC_BYTE_NUM_OUT > 0                    //-- if we have to transfer more then 64 bytes --
                                              //-- we must do another round -------------------
                                              //-- to transfer the remainig bytes -------------


    do                                                          // wait for the data to be       
    {                                                           // transferred from the output  
      TempLong.Long = SPIReadRegisterDirect(ECAT_PRAM_RD_CMD,2);// process ram to the read fifo 
    }                                                           //    
    while (TempLong.Byte[1] != SEC_BYTE_NUM_ROUND_OUT/4);       // *CCC*  

    SCS_Low_macro                                               // enable SPI chip select   
    
    SPI_Transfer(COMM_SPI_READ);                              // SPI read command
    SPI_Transfer(0x00);                                       // address of the read  
    SPI_Transfer(0x00);                                   // fifo MsByte first
    
    for (i=0; i< (SEC_BYTE_NUM_ROUND_OUT); i++)                 // transfer loop for the remaining 
    {                                                           // bytes
      EasyCAT_BufferOut.Byte[i+64] = SPI_Transfer(DUMMY_BYTE);        // we transfer the second part of
    }                                                           // the buffer, so offset by 64
      
    SCS_High_macro                                              // SPI chip select disable  
  #endif  
}  


//---- write to the process ram fifo --------------------------------------------------------------

void SPIWriteProcRamFifo()    // write data to the input process ram, through the fifo
                                       //    
                                       // these are the bytes that we have read from the inputs of our                   
                                       // application and that will be sent to the EtherCAT master
{
  ULONG TempLong;
  unsigned char i;
  
  
  #if TOT_BYTE_NUM_IN > 0  
  
    SPIWriteRegisterDirect (ECAT_PRAM_WR_CMD, PRAM_ABORT);        // abort any possible pending transfer
  
 
    SPIWriteRegisterDirect (ECAT_PRAM_WR_ADDR_LEN, (0x00001200 | (((uint32_t)TOT_BYTE_NUM_IN) << 16)));   
                                                                  // the high word is the num of bytes
                                                                  // to write 0xTOT_BYTE_NUM_IN----
                                                                  // the low word is the input process        
                                                                  // ram offset  0x----1200
                                                                                               
    SPIWriteRegisterDirect (ECAT_PRAM_WR_CMD, 0x80000000);        // start command  
  
  
                                                //------- one round is enough if we have ----
                                                //------- to transfer up to 64 bytes --------
    
    do                                                            // check that the fifo has      
    {                                                             // enough free space 
      TempLong.Long = SPIReadRegisterDirect (ECAT_PRAM_WR_CMD,2); //  
    }                                                             //  
    while (TempLong.Byte[1] < (FST_BYTE_NUM_ROUND_IN / 4));       //    * this can fail and hang here *
  
    SCS_Low_macro                                                 // enable SPI chip select
  
    SPI_Transfer(COMM_SPI_WRITE);                               // SPI write command
    SPI_Transfer(0x00);                                         // address of the write fifo 
    SPI_Transfer(0x20);                                         // MsByte first 

    for (i=0; i< (FST_BYTE_NUM_ROUND_IN - 1 ); i++)               // transfer the data
    {                                                             //
      SPI_Transfer (EasyCAT_BufferIn.Byte[i]);                          //      
    }                                                             //
                                                                  //  
    SPI_Transfer (EasyCAT_BufferIn.Byte[i]);                        // one last byte
  
    SCS_High_macro                                                // disable SPI chip select           
  #endif        

  
  #if SEC_BYTE_NUM_IN > 0                     //-- if we have to transfer more then 64 bytes --
                                              //-- we must do another round -------------------
                                              //-- to transfer the remainig bytes -------------

    do                                                          // check that the fifo has     
    {                                                           // enough free space       
      TempLong.Long = SPIReadRegisterDirect(ECAT_PRAM_WR_CMD,2);// 
    }                                                           //  
    while (TempLong.Byte[1] < (SEC_BYTE_NUM_ROUND_IN/4));       //   *CCC*
                             
    SCS_Low_macro                                               // enable SPI chip select
    
    SPI_Transfer(COMM_SPI_WRITE);                             // SPI write command
    SPI_Transfer(0x00);                                       // address of the write fifo 
    SPI_Transfer(0x20);                                       // MsByte first 

    for (i=0; i< (SEC_BYTE_NUM_ROUND_IN - 1); i++)              // transfer loop for the remaining 
    {                                                           // bytes
      SPI_Transfer (EasyCAT_BufferIn.Byte[i+64]);                     // we transfer the second part of
    }                                                           // the buffer, so offset by 64
                                                                //  
    SPI_Transfer (EasyCAT_BufferIn.Byte[i+64]);                   // one last byte  

    SCS_High_macro                                              // disable SPI chip select    
  #endif     
}


void SPIEnd()
{
    SCS_High_macro

    SPI_Cmd( SPIX_ESC, DISABLE );
}
