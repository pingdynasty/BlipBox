#include "eeprom.h"

uint8_t 
eeprom_read_byte (const uint8_t *addr){
  return 0;
}

uint16_t
eeprom_read_word (const uint16_t *addr){
  return 0;
}

void
eeprom_read_block (void *pointer_ram,
                   const void *pointer_eeprom,
                   size_t size){
}

void
eeprom_write_byte (uint8_t *addr,uint8_t value){
}

void
eeprom_write_word (uint16_t *addr,uint16_t value){
}

void
eeprom_write_block (const void *pointer_ram,
                    void *pointer_eeprom,
                    size_t size){
}
