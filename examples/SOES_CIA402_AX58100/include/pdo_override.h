#ifndef __PDO_OVERRIDE_H__
#define __PDO_OVERRIDE_H__


/* DMA for SPI1 RX, TX
    Stream 0 is already used for ADC
    Stream 5 is used for sserial */

#define ESC_CMD_READ        0x02
#define ESC_CMD_READ_WAIT_STATE 0x03
#define ESC_CMD_WRITE       0x04

#define READ_END_BYTE       0xFF

#define ADDR_SIZE           2               // 2 bytes mode: addr < 3 | cmd
#define WAIT_SIZE           1               // Wait State byte, single
#define RX_PADDING_SIZE     8 - ADDR_SIZE - WAIT_SIZE
#define TX_PADDING_SIZE     8 - ADDR_SIZE   // COE_pdo(Un)pack wants buffers aligned to 64 bit grid
                                            // when address is setting, read is ALevent that it is not going to COE
                                            // so buffers are larger by offset so COE part starts on even address
#define PDO_SIZE            sizeof(Obj.Control_Word) + sizeof(Obj.Target_position) // SM2_sml
#define PDO_TR_SIZE         PDO_SIZE + ADDR_SIZE + WAIT_SIZE 

enum DMA_PDI_transmission_state_t { NOT_STARTED = 0, IN_PROGRESS = 1 } typedef DMA_PDI_transmission_state_t;


void init_override(void);
void rxpdo_override(void);
void txpdo_override(void);

void on_rxpdo_transfer_finished(void);
void on_txpdo_transfer_finished(void);


#endif // __PDO_OVERRIDE_H__