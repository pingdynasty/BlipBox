#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <inttypes.h>
#include "globals.h"

// parameter IDs occupy four bits
#define BRIGHTNESS_PARAMETER_ID       (0x01 << 2)
#define SENSITIVITY_PARAMETER_ID      (0x02 << 2)
#define TLC_GSCLK_PERIOD_PARAMETER_ID (0x03 << 2)
#define SERIAL_SPEED_PARAMETER_ID     (0x04 << 2)
#define FOLLOW_MODE_PARAMETER_ID      (0x05 << 2)

/* #define BRIGHTNESS_PARAMETER_ID       (0x01 << 2) */
/* #define SENSITIVITY_PARAMETER_ID      (0x02 << 2) */
/* #define FOLLOW_MODE_PARAMETER_ID      (0x03 << 2) */
/* #define X_MIN_PARAMETER_ID            (0x04 << 2) */
/* #define X_RANGE_PARAMETER_ID          (0x05 << 2) */
/* #define Y_MIN_PARAMETER_ID            (0x06 << 2) */
/* #define Y_RANGE_PARAMETER_ID          (0x07 << 2) */
/* #define SERIAL_SPEED_PARAMETER_ID     (0x08 << 2) */

#define PARAMETER_ID_MASK         0x3c  // 00111100
#define PARAMETER_VALUE_MASK      0x3ff // 00000011 11111111
/* #define PARAMETER_VALUE_MASK      0x03 // 00000011 */

void setParameter(uint16_t data);
uint16_t getParameter(uint8_t pid);

/* // there's 512 bytes of EEPROM on the AtMega168, enough for 256 2-byte parameters */
/* // the bitspace allowed for parameter id's is only 4 bits though */
/* void setParameter(uint8_t parameterId, uint16_t value); */
/* uint16_t getParameter(uint8_t parameterId); */

#endif /* _PARAMETERS_H_ */
