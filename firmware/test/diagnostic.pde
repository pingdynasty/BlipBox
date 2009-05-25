
// The 5-wire touch screen panel works by applying a voltage
// at the corners of the bottom resistive layer and measuring the
// vertical or horizontal resistive network with the wiper

// A measurement of the Y position
// of the pointing device is made by connecting the upper left
// and upper right corners of the resistive layer to V+ and the
// lower left and lower right corners to ground.

// For the horizontal measurement, the upper left corner
// and lower left corner are connected to ground and the upper
// right and lower right corners are connected to V+

#include "LedController.h"
#include "KeyController.h"
#include "defs.h"

// cable left, x / y positions
// 170 850 top right
// 170 160 bottom right
// 830 840 top left
// 830 160 bottom left

// PC1 - RT / pin 1
// PC0 - RL / pin 2
// PC2 - SG / pin 3
// PC3 - LT / pin 4
// PC4 - LL / pin 5

// RT pin 1 <-> +5
// RL pin 2 <-> digital 4
// SG pin 3 <-> analog 0
// LT pin 4 <-> digital 3
// LL pin 5 <-> GND

#define FRAME_DELAY 100

KeyController keys;

void setup() {
  Serial.begin(DEFAULT_SERIAL_SPEED);
  Serial.println("setup started");
  keys.init();
  Serial.println("setup complete");
}

void keychange(uint8_t col, uint8_t row, bool ison){
  Serial.print("keychange ");
  Serial.print(col, DEC);
  Serial.print(" \t");
  Serial.print(row, DEC);
  Serial.print("\t: ");
  Serial.println(ison);
}

void printRowCol(){
  Serial.print(keys.readRow(), DEC);
  Serial.print(" \t");
  Serial.println(keys.readColumn(), DEC);
}

void printRaw(){
  Serial.print(keys.check(), DEC);
  Serial.print(",");
  Serial.print(keys.readX(), DEC);
  Serial.print(",");
  Serial.println(keys.readY(), DEC);
}

void loop() {
  printRaw();
//   if(keys.keyscan())
//     printRowCol();
//   if(keys.check() < 1000)
//     printRowCol();
//   keys.keyscan();
  delay(FRAME_DELAY);

}
