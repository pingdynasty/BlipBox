#ifndef _DEFS_H_
#define _DEFS_H_

/* #define BLIPBOX_V6 */
#define BLIPBOX_V9
/* #define BLIPBOX_P4 */
/* #define MONOTAR_P1 */

#define MIDI_SERIAL_SPEED 31250L
/* #define MIDI_SERIAL_SPEED 57600L */

// used for inverting sensor input values: 10 bits max value
#define SENSOR_MAX 1023

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L

// set data transfer mode for the TLC5940: serial bitbang or hardware SPI
#define TLC_SPI 1
#define TLC_BITBANG 2
#define TLC_DATA_TRANSFER_MODE TLC_SPI

// ATMega168 has 512 bytes EEPROM
#define MIDI_PRESET_OFFSET 32 // arbitrary 32 bytes for configuration
#define MIDI_ZONE_PRESET_SIZE 4
#define MIDI_ZONES_IN_PRESET 8
#define MIDI_ZONE_PRESETS 5

#endif /* _DEFS_H_ */
