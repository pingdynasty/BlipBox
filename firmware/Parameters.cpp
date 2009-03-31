#include <avr/eeprom.h>
#include <inttypes.h>
#include "Parameters.h"

#define PID_LSB(pid) ((pid >> 2) * 2)
#define PID_MSB(pid) (((pid >> 2) * 2) + 1)

#define EEPROM_READ(pid) eeprom_read_byte((unsigned char*)pid)
#define EEPROM_WRITE(pid, value) eeprom_write_byte((unsigned char*)pid, value)

void setParameter(uint8_t pid, uint16_t value){
  EEPROM_WRITE(PID_LSB(pid), value >> 8);
  EEPROM_WRITE(PID_MSB(pid), value & 0xff);
}

uint16_t getParameter(uint8_t pid){
  return (EEPROM_READ(PID_LSB(pid)) << 8) | EEPROM_READ(PID_MSB(pid));
}
