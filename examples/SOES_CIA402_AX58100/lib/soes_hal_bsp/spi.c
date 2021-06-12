#include "spi.h"


void spi_gpio_setup(void)
{
    GPIO_InitTypeDef gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,  GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    /* SPI SCK pin configuration */
    gpio.GPIO_Pin   = GPIO_Pin_5;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &gpio);

    /* SPI  MISO pin configuration */
    gpio.GPIO_Pin   = GPIO_Pin_6;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &gpio);  

    /* SPI  MOSI pin configuration */
    gpio.GPIO_Pin   = GPIO_Pin_7;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &gpio);

    /* CS */
    gpio.GPIO_Pin   = GPIO_Pin_4; 
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);
}

void spi_setup(void)
{
    spi_gpio_setup();
    
    SPI_InitTypeDef  spi;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_I2S_DeInit(SPI1);
    spi.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize          = SPI_DataSize_8b;
    spi.SPI_CPOL              = SPI_CPOL_Low;
    spi.SPI_CPHA              = SPI_CPHA_1Edge;
    spi.SPI_NSS               = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    spi.SPI_FirstBit          = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial     = 7;
    spi.SPI_Mode              = SPI_Mode_Master;
    SPI_Init(SPI1, &spi);

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) { }

    /* Enable the SPI peripheral */
    SPI_Cmd(SPI1, ENABLE);
}

void spi_select (int8_t board)
{
    // Soft CSM
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    GPIO_ResetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #elif SCS_ACTIVE_POLARITY == SCS_HIGH
    GPIO_SetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #endif
}

void spi_unselect (int8_t board)
{
    #if SCS_ACTIVE_POLARITY == SCS_LOW
	GPIO_SetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #elif SCS_ACTIVE_POLARITY == SCS_HIGH
    GPIO_ResetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #endif
}

inline static uint8_t spi_transfer(uint8_t byte)
{
    SPI_I2S_SendData(SPIX_ESC, byte);

    while ( SPI_I2S_GetFlagStatus(SPIX_ESC, SPI_I2S_FLAG_RXNE) == RESET) { }
    // TODO add timeout

    return SPI_I2S_ReceiveData(SPIX_ESC);
}

void spi_write (int8_t board, uint8_t *data, uint8_t size)
{
    for(int i = 0; i < size; ++i)
    {
        spi_transfer(data[i]);
    }
}

void spi_read (int8_t board, uint8_t *result, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer(DUMMY_BYTE);
    }
}


void spi_bidirectionally_transfer (int8_t board, uint8_t *result, uint8_t *data, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer(data[i]);
    }
}
