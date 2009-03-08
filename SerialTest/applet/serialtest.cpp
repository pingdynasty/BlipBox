#include "WProgram.h"
// #include <Wire.h>

#define SERIAL_SPEED 115200

// #define DELAY 100

#define CTS_PIN 7
#define LED_PIN 8

uint8_t incoming;

void setup() {
  Serial.begin(SERIAL_SPEED);

#ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
#endif // LED_PIN

#ifdef CTS_PIN
  pinMode(CTS_PIN, OUTPUT);
#endif // CTS_PIN

}

void loop() {
#ifdef CTS_PIN
    digitalWrite(CTS_PIN, LOW);
#endif // CTS_PIN
  if(serialAvailable()){
#ifdef LED_PIN
    digitalWrite(LED_PIN, HIGH);
#endif // LED_PIN
    incoming = serialRead();
    serialWrite(incoming);
#ifdef CTS_PIN
    digitalWrite(CTS_PIN, HIGH);
#endif // CTS_PIN
  }else{
#ifdef LED_PIN
    digitalWrite(LED_PIN, LOW);
#endif // LED_PIN
  }
#ifdef DELAY
  delay(DELAY);
#endif // DELAY
}
int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

