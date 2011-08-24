#include "spi.h"

#include <avr/io.h>

// CS slave select pin
// SCK clock pin
// SDI MOSI pin: master out, slave in
#define SPI_SDI_DDR   DDRB
#define SPI_SCK_DDR   DDRB
#define SPI_CS_DDR    DDRC
#define SPI_SDI_PORT  PORTB
#define SPI_SCK_PORT  PORTB
#define SPI_CS_PORT   PORTC
#define SPI_SDI_PIN   PB4
#define SPI_SCK_PIN   PB0
#define SPI_CS_PIN    PC5

void spi_init() {
  SPI_CS_DDR |= _BV(SPI_CS_PIN);
  SPI_SCK_DDR |= _BV(SPI_SCK_PIN);
  SPI_SDI_DDR |= _BV(SPI_SDI_PIN);
  SPI_CS_PORT |= _BV(SPI_CS_PIN);
  SPI_CS_PORT |= _BV(SPI_CS_PIN);
  SPI_SDI_PORT &= ~_BV(SPI_SDI_PIN);
  SPI_SCK_PORT &= ~_BV(SPI_SCK_PIN);
}

void spi_cs_toggle() {
  SPI_CS_PORT ^= _BV(SPI_CS_PIN);
}

void spi_cs(bool selected) {
  if(selected)
    SPI_CS_PORT |= _BV(SPI_CS_PIN);
  else
    SPI_CS_PORT &= ~_BV(SPI_CS_PIN);
}

void spi_send(uint8_t data){
  for(uint8_t bit = 0x80; bit; bit >>= 1){
    if(data & bit)
      SPI_SDI_PORT |= _BV(SPI_SDI_PIN);
    else
      SPI_SDI_PORT &= ~_BV(SPI_SDI_PIN);
    SPI_SCK_PORT |= _BV(SPI_SCK_PIN);
    SPI_SCK_PORT &= ~_BV(SPI_SCK_PIN);
  }
}
