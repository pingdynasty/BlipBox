#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/* #include <avr/wdt.h> */
#include "LedController.h"
#include "KeyController.h"

extern LedController leds;
extern KeyController keys;

// some global parameters that really should be dealt more intelligently
extern uint16_t touchscreen_x_min;
extern uint16_t touchscreen_x_range;
extern uint16_t touchscreen_y_min;
extern uint16_t touchscreen_y_range;


// error codes
#define MESSAGE_READ_ERROR 4
#define MODE_ERROR 6

// blocking call to serialRead
uint8_t readByte();
void error(uint8_t code);

void setmode(uint8_t mode); // sets the mode to be used on next system reset
void reset(uint8_t mode); // reset device into new or same mode

// note that the wdt has to be disabled on startup after this has been called

// disable the watchdog after a reset:
// Function Prototype
void disable_watchdog();
// attributes cause device to hang or reboot?
/* void disable_watchdog() __attribute__((naked)) __attribute__((section(".init3"))); */
/* void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3"))); */

#endif /* _GLOBALS_H_ */
