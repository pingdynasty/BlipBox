#ifndef _SPI_H_
#define _SPI_H_

#include <inttypes.h>

#define DAC_A_B_BIT  7 // 1 = Write to A, 0 = Write to B
#define DAC_BUF_BIT  6 // 1 = Buffered, 0 = Unbuffered
#define DAC_GA_BIT   5 // 1 = 1x Gain, 0 = 2x Gain
#define DAC_SHDN_BIT 4 // 1 = Output buffer enabled, 0 = Output buffer disabled

void spi_init();
void spi_cs(bool selected);
void spi_cs_toggle();
void spi_send(uint8_t data);

#endif /* _SPI_H_ */
