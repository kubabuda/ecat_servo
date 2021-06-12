#include <string.h>
#include "stm32f4xx.h"
#include "spi.h"
#include "usart.h"

#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"

#include "pdo_override.h"


static int et1100 = 1;

static volatile DMA_PDI_transmission_state_t pdi_dma_transmission = NOT_STARTED;

/* SPI DMA RX buffer */
uint8_t pdo_rxbuf[PDO_TR_SIZE + RX_PADDING_SIZE] __attribute__((aligned (8)));
uint8_t * pdo_spi_rxbuf = pdo_rxbuf + RX_PADDING_SIZE;
uint8_t * pdo_coe_rxbuf = pdo_rxbuf + RX_PADDING_SIZE + ADDR_SIZE + WAIT_SIZE;

/* SPI DMA TX buffer  */
uint8_t pdo_txbuf[PDO_TR_SIZE + TX_PADDING_SIZE] __attribute__((aligned (8))) = { 0 };
uint8_t * pdo_spi_txbuf = pdo_txbuf + TX_PADDING_SIZE;
uint8_t * pdo_coe_txbuf = pdo_txbuf + TX_PADDING_SIZE + ADDR_SIZE;


void spi_dma_prepare_transmission(uint16_t address, uint8_t * tx_buffer, uint8_t cmd)
{
    // set address and command
    tx_buffer[0] = address >> 5;
    tx_buffer[1] = ((address & 0x1F) << 3) | cmd;

    if (cmd == ESC_CMD_READ_WAIT_STATE) {
        // set wait state byte
        tx_buffer[ADDR_SIZE] = READ_END_BYTE; 
        // set read termination
        tx_buffer[PDO_TR_SIZE - 1] = READ_END_BYTE;
    } else if (cmd == ESC_CMD_READ) {
        // set read termination
        tx_buffer[PDO_TR_SIZE - 1] = READ_END_BYTE;
    }
}


void DMA2_Stream2_IRQHandler(void)
{
    /* Test on DMA Stream Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
    {
        /* Clear DMA Stream Transfer Complete interrupt pending bit */
        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
        pdi_dma_transmission = NOT_STARTED;
    }
}


void spi_dma_setup(void)
{
    /* Enable DMA clock*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
	/* Deinitialize DMA Streams */
	DMA_DeInit(DMA2_Stream3);
	DMA_DeInit(DMA2_Stream2);

    while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE) { }
    while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE) { }

    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_BufferSize = (uint16_t)(PDO_TR_SIZE);
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR));
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    /* Configure Tx DMA */
    DMA_InitStructure.DMA_Channel = DMA_Channel_3;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pdo_spi_txbuf;
    DMA_Init(DMA2_Stream3, &DMA_InitStructure);

    /* Configure Rx DMA */
    DMA_InitStructure.DMA_Channel = DMA_Channel_3;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pdo_spi_rxbuf;
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);

    /* Enable the DMA Stream IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void spi_dma_start(void)
{
    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);

    /* Enable the SPI Rx DMA request */
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

    spi_select (et1100);
    /* Enable the DMA SPI TX Stream */
    DMA_Cmd(DMA2_Stream3, ENABLE);
    /* Enable the DMA SPI RX Stream */
    DMA_Cmd(DMA2_Stream2, ENABLE);
    
    pdi_dma_transmission = IN_PROGRESS;
}


void spi_dma_stop(void)
{
    spi_unselect (et1100);

	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);

	DMA_Cmd(DMA2_Stream3, DISABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
}


void spi_dma_rxpdo_start(void)
{
    memset(pdo_coe_txbuf, 0, PDO_SIZE);  /* make sure TX buffer is filled with 0s, no read end byte */
    spi_dma_prepare_transmission(ESC_SM2_sma, pdo_spi_txbuf, ESC_CMD_READ_WAIT_STATE);

    spi_dma_start();
}


void spi_dma_txpdo_start()
{
    spi_dma_prepare_transmission(ESC_SM3_sma, pdo_spi_txbuf, ESC_CMD_WRITE);

    spi_dma_start();
}


void init_override(void)
{
    spi_dma_setup();
    spi_setup();
    
    /* SMx_sml are calculated at runtime on PREOP_TO_SAFEOP, buffer size is needed on compilation
        TODO test if they match, when ESC is OP */
    // CC_ASSERT (ESCvar.ESC_SM2_sml == PDO_SIZE);
    // CC_ASSERT (ESCvar.ESC_SM3_sml == PDO_SIZE);
}


void rxpdo_override(void)
{
    spi_dma_rxpdo_start();
    while (pdi_dma_transmission == IN_PROGRESS) { } /* polling for "end of transfer" flag */
    on_rxpdo_transfer_finished();
}


void on_rxpdo_transfer_finished(void)
{
    spi_dma_stop();
    ESCvar.ALevent = etohs ((uint16_t)*pdo_spi_rxbuf);
    
    if (MAX_MAPPINGS_SM2 > 0)
    {
        COE_pdoUnpack (pdo_coe_rxbuf, ESCvar.sm2mappings, SMmap2);
    }
}


void txpdo_override(void)
{
    if (MAX_MAPPINGS_SM3 > 0)
    {
        COE_pdoPack (pdo_coe_txbuf, ESCvar.sm3mappings, SMmap3);
    }

    spi_dma_txpdo_start();
    while (pdi_dma_transmission == IN_PROGRESS) { } /* polling to end of transfer flag */
    on_txpdo_transfer_finished();
}


void on_txpdo_transfer_finished(void)
{
    spi_dma_stop();
    ESCvar.ALevent = etohs ((uint16_t)*pdo_spi_rxbuf);
}