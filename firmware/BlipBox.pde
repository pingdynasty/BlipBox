// some current measurements on prototype I:
// 4.88mA all leds on, 9v battery power
// 4.79mA all leds on, usb power
// 0.44 no leds on
// 68k resistor

// 184mA all on
// 14.2mA all off, 17.2 one on
// 474Ohm resistor

// much less difference (none?) between single led brightness and whole row when using high (68k) resistor

// arduino max DC current per I/O pin : 40mA
// #include <avr/eeprom.h>
#include "BlipBoxProtocol.h"
// #include "MonomeSeriesProtocol.h"
#include "globals.h"
#include "defs.h"

#include "Tlc5940.h"

Protocol* protocol;
BlipBoxProtocol blipboxProtocol;

// Tlc5940 leds;

// uint8_t rowpins[] = {2, 4, 7, 8, 6};
// uint8_t rowcount = 5;

void setup() {
//   pinMode(12, OUTPUT);
//   digitalWrite(12, HIGH);

//   for(uint8_t i=0; i<rowcount; ++i)
//     pinMode(rowpins[i], OUTPUT);

//   for(uint8_t i=0; i<rowcount; ++i)
//     digitalWrite(rowpins[i], HIGH);

//   disable_watchdog(); // disable watchdog timer
  // wdt_init causes device to hang? setup gets stuck?

//   pinMode(BUTTON1_PIN, INPUT);

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
  leds.init();
}

void loop() {
  protocol->process();
}

// void loop() {
//   int direction = 1;
//   for (int channel = 0; channel < NUM_TLCS * 16; channel += direction) {
//     leds.clear();
//     if (channel == 0) {
//       direction = 1;
//     } else {
//       leds.set(channel - 1, 1000);
//     }
//     leds.set(channel, 4095);
//     if (channel != NUM_TLCS * 16 - 1) {
//       leds.set(channel + 1, 1000);
//     } else {
//       direction = -1;
//     }
//     leds.update();
//     delay(75);
//   }
// }
