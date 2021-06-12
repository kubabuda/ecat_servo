#ifndef SRC_APP_SPI_H_
#define SRC_APP_SPI_H_

#include <stdint.h>

#define SCS_LOW                           0
#define SCS_HIGH                          1
#define SCS_ACTIVE_POLARITY               SCS_LOW

#define SPIX_ESC                          SPI1
#define SPIX_ESC_SPEED                    18000000
#define ESC_GPIO_Pin_CS                   PA4


#define DUMMY_BYTE 0xFF


extern "C" void spi_setup(void);
extern "C" void spi_select (int8_t board);
extern "C" void spi_unselect (int8_t board);
extern "C" void write (int8_t board, uint8_t *data, uint8_t size);
extern "C" void read (int8_t board, uint8_t *result, uint8_t size);
extern "C" void spi_bidirectionally_transfer (int8_t board, uint8_t *result, uint8_t *data, uint8_t size);


#endif /* SRC_APP_SPI_H_ */
