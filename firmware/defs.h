#ifndef _DEFS_H_
#define _DEFS_H_

#define BLIPBOX_V9
/* #define BLIPBOX_V8 */
/* #define BLIPBOX_P4 */
/* #define MONOTAR_P1 */

// used for inverting sensor input values: 10 bits max value
#define SENSOR_MAX 1023

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L

#define ROWS 5
#define COLS 16
#define FRAME_LENGTH 16 // 16 for serial connection

#endif /* _DEFS_H_ */
