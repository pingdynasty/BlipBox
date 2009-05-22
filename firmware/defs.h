#ifndef _DEFS_H_
#define _DEFS_H_

/* #define DEFAULT_SERIAL_SPEED 9600 */
#define DEFAULT_SERIAL_SPEED 115200

/* #define SENSITIVITY 120 // the lower the value, the less sensitive */
/* #define SENSITIVITY 600 // the lower the value, the less sensitive */
#define SENSITIVITY 200 // the lower the value, the less sensitive

#define BRIGHTNESS 0xff

/* #define SENSE_DELAY 10 */
/* sleep time between configuring and reading touch screen values */
#define SENSE_DELAY 1

// used for inverting sensor input values: 10 bits max value
#define SENSOR_MAX 1023

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L

#define ROWS 5
#define COLS 16
#define FRAME_LENGTH 16 // 16 for serial connection

// eeprom address offsets
#define EEPROM_SERIAL_SPEED_ADDRESS (uint8_t*)0
#define EEPROM_MODE_ADDRESS         (uint8_t*)1

// eeprom values
#define EEPROM_MODE_BLIPBOX         1
#define EEPROM_MODE_MONOME_SERIES   2

#endif /* _DEFS_H_ */
