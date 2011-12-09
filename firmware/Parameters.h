#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <inttypes.h>
#include "globals.h"

// parameter IDs occupy four bits
#define BRIGHTNESS_PARAMETER_ID       (0x01 << 2)
#define SENSITIVITY_PARAMETER_ID      (0x02 << 2)
#define TLC_GSCLK_PERIOD_PARAMETER_ID (0x03 << 2)
#define SERIAL_SPEED_PARAMETER_ID     (0x04 << 2)
#define PRESET_PARAMETER_ID           (0x05 << 2)
#define MIDI_ZONE_PARAMETER_ID        (0x06 << 2)
#define X_MIN_PARAMETER_ID            (0x07 << 2)
#define X_RANGE_PARAMETER_ID          (0x08 << 2)
#define Y_MIN_PARAMETER_ID            (0x09 << 2)
#define Y_RANGE_PARAMETER_ID          (0x0a << 2)
#define CV1_PARAMETER_ID              (0x0b << 2)
#define CV2_PARAMETER_ID              (0x0c << 2)
#define CV3_PARAMETER_ID              (0x0d << 2)
#define CV4_PARAMETER_ID              (0x0e << 2)
#define VERSION_PARAMETER_ID          (0x0f << 2)

void setParameter(uint8_t pid, uint16_t value);
uint16_t getParameter(uint8_t pid);

/* // there's 512 bytes of EEPROM on the AtMega168, enough for 256 2-byte parameters */
/* // the bitspace allowed for parameter id's is only 4 bits though */
/* void setParameter(uint8_t parameterId, uint16_t value); */
/* uint16_t getParameter(uint8_t parameterId); */

#endif /* _PARAMETERS_H_ */
