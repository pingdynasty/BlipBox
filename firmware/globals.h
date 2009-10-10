#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/* #include <avr/wdt.h> */
#include "LedController.h"
#include "DisplayManager.h"
#include "KeyController.h"
#include "Animator.h"
#include "Configuration.h"

extern LedController leds;
extern DisplayManager display;
extern KeyController keys;
extern SignalAnimator signal;
extern Configuration config;

// some global parameters that really should be dealt more intelligently
extern uint16_t touchscreen_x_min;
extern uint16_t touchscreen_x_range;
extern uint16_t touchscreen_y_min;
extern uint16_t touchscreen_y_range;

// error codes
// the code corresponds to the number of blinks emmitted to signal the error
#define MESSAGE_READ_ERROR 2
#define MODE_ERROR 3

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
