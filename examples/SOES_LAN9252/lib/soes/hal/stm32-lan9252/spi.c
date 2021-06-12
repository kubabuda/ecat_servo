#include "spi.h"


void spi_setup(void)
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
    spi.SPI_CPOL              = SPIX_ESC_CPOL;
    spi.SPI_CPHA              = SPIX_ESC_CPHA;
    spi.SPI_NSS               = SPIX_ESC_SCS;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    spi.SPI_FirstBit          = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial     = 7;
    spi.SPI_Mode              = SPI_Mode_Master;
    SPI_Init(SPIX_ESC, &spi);

    /* Enable the SPI peripheral */
    SPI_Cmd(SPIX_ESC, ENABLE);
}

void spi_select (int8_t board)
{
    // Soft CSM
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    GPIO_ResetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #endif
}

void spi_unselect (int8_t board)
{
    #if SCS_ACTIVE_POLARITY == SCS_LOW
	GPIO_SetBits(ESC_GPIOX_CS, ESC_GPIO_Pin_CS);
    #endif
}

inline static uint8_t spi_transfer(uint8_t byte)
{
    SPIX_ESC->DR = byte;
    while ((SPIX_ESC->SR & SPI_I2S_FLAG_RXNE) == RESET);
    return SPIX_ESC->DR;
}

void write (int8_t board, uint8_t *data, uint8_t size)
{
    for(int i = 0; i < size; ++i)
    {
        spi_transfer(data[i]);
    }
}

void read (int8_t board, uint8_t *result, uint8_t size)
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
