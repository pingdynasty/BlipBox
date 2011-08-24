#ifndef _SPI_H_
#define _SPI_H_

#include <inttypes.h>

void spi_init();
void spi_cs(bool selected);
void spi_cs_toggle();
void spi_send(uint8_t data);

#endif /* _SPI_H_ */
