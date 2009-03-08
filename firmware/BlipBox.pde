// some current measurements on prototype I:
// 4.88mA all leds on, 9v battery power
// 4.79mA all leds on, usb power
// 0.44 no leds on
// 68k resistor

// 184mA all on
// 14.2mA all off, 17.2 one on
// 474Ohm resistor

// much less difference (none?) between single led brightness and whole row when using high (68k) resistor


// pins
#define POT_PIN 5
#define BUTTON1_PIN 2


// arduino max DC current per I/O pin : 40mA
#include <avr/eeprom.h>
#include "BlipBoxProtocol.h"
#include "MonomeSeriesProtocol.h"
#include "globals.h"
#include "defs.h"

Protocol* protocol;
// MonomeSeriesProtocol monomeseriesProtocol;
BlipBoxProtocol blipboxProtocol;

void setup() {
//   disable_watchdog(); // disable watchdog timer
  // wdt_init causes device to hang? setup gets stuck?

  pinMode(BUTTON1_PIN, INPUT);

//   if(digitalRead(BUTTON1_PIN))
//     protocol = &monomeseriesProtocol;
//   else
//     protocol = &blipboxProtocol;

  // set blipbox or monome mode
//   uint8_t val = eeprom_read_byte(EEPROM_MODE_ADDRESS);
//   if(val == EEPROM_MODE_MONOME_SERIES)
//     protocol = &monomeseriesProtocol;
//   else
    protocol = &blipboxProtocol;

  protocol->init();
}

void loop() {
  protocol->process();
}
