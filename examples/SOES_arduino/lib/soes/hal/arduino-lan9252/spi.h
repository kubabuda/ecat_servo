#ifndef SRC_APP_SPI_H_
#define SRC_APP_SPI_H_

#include <stdint.h>


void spi_setup(void);
void spi_select (int8_t board);
void spi_unselect (int8_t board);
void write (int8_t board, uint8_t *data, uint8_t size);
void read (int8_t board, uint8_t *result, uint8_t size);
void spi_bidirectionally_transfer (int8_t board, uint8_t *result, uint8_t *data, uint8_t size);


#endif /* SRC_APP_SPI_H_ */
