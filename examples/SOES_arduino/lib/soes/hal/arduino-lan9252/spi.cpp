#include "spi.hpp"
// #include <Arduino.h>
#include <SPI.h>


char SCS = ESC_GPIO_Pin_CS;


void spi_setup(void)
{
  SPI.begin();    
  pinMode(SCS, OUTPUT);   
  spi_unselect(0);
  delay(100);     
  SPI.beginTransaction(SPISettings(SPIX_ESC_SPEED, MSBFIRST, SPI_MODE0)); 

}

void spi_select (int8_t board)
{
    // Soft CSN
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    digitalWrite(SCS, LOW);
    #endif
}

void spi_unselect (int8_t board)
{
    // Soft CSN
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    digitalWrite(SCS, HIGH);
    #endif
}

inline static uint8_t spi_transfer_byte(uint8_t byte)
{
    return SPI.transfer(byte);
    // AVR will need handling last byte transfer difference,
    // but then again they pobably wont even fit EtherCAT stack in RAM
    // so no need to care for now
}

void write (int8_t board, uint8_t *data, uint8_t size)
{
    for(int i = 0; i < size; ++i)
    {
        spi_transfer_byte(data[i]);
    }
}

void read (int8_t board, uint8_t *result, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer_byte(DUMMY_BYTE);
    }
}


void spi_bidirectionally_transfer (int8_t board, uint8_t *result, uint8_t *data, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer_byte(data[i]);
    }
}
