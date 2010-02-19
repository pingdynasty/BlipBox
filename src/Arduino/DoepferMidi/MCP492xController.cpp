#include "MCP492xController.h"

#include "spi.h"
#include <avr/io.h>

#define DAC_A_B_BIT  7
#define DAC_BUF_BIT  6   // 0 = unbuffered, 1 = buffered
#define DAC_GA_BIT   5   // 0 = 2x gain,    1 = 1x gain
#define DAC_SHDN_BIT 4

#define TRANSFER_BITS _BV(DAC_SHDN_BIT) // 2x gain
// #define TRANSFER_BITS (_BV(DAC_SHDN_BIT) | _BV(DAC_GA_BIT)) // 1x gain

void MCP492xController::init(){
  spi_init();
}

void MCP492xController::send(uint8_t data){
  spi_out(TRANSFER_BITS, data & 0xff);
}

void MCP492xController::send(uint8_t high, uint8_t low){
  spi_out(TRANSFER_BITS | (high & 0xf), low & 0xff);
}

void MCP492xController::send(uint16_t data){
  spi_out(TRANSFER_BITS | (data >> 8 & 0xf), data & 0xff);
}
