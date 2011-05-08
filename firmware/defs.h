#ifndef _DEFS_H_
#define _DEFS_H_

/* #define BLIPBOX_V6 */
#define BLIPBOX_V9
/* #define BLIPBOX_P4 */
/* #define MONOTAR_P1 */

// used for inverting sensor input values: 10 bits max value
#define SENSOR_MAX 1023

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L

#define ROWS 5
#define COLS 16
#define FRAME_LENGTH 16 // 16 for serial connection

// set data transfer mode for the TLC5940: serial bitbang or hardware SPI
#define TLC_SPI 1
#define TLC_BITBANG 2
// #ifdef BLIPBOX_V6
#define TLC_DATA_TRANSFER_MODE TLC_SPI
// #else
// #define TLC_DATA_TRANSFER_MODE TLC_BITBANG
// #endif

#endif /* _DEFS_H_ */
