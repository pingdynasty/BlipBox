#include "globals.h"
#include <wiring.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

// LedMatrix leds;
LedController leds;
KeyController keys;
SignalAnimator signal;
Configuration config;

// macro to perfom a soft reset using the watchdog timer
// #define soft_reset()        \
// do                          \
// {                           \
//     wdt_enable(WDTO_15MS);  \
//     for(;;)                 \
//     {
//     }
// } while(0)

// Disable watchdog: Function Implementation
// void wdt_init(void) {
// void disable_watchdog(void) {
// //     MCUSR = 0;
//         cli();
//         /* Clear WDRF in MCUSR */
//         MCUSR &= ~(1<<WDRF);
//         /* Write logical one to WDCE and WDE */
//         /* Keep old prescaler to prevent unintentional time-out */
//         WDTCSR |= (1<<WDCE) | (1<<WDE);
//         /* Turn off WDT */
//         WDTCSR = 0x00;

//     wdt_disable();
//         sei();
// }



// void WDT_off(void)
// {
//         __disable_interrupt();
//         __watchdog_reset();
//         /* Clear WDRF in MCUSR */
//         MCUSR &= ~(1<<WDRF);
//         /* Write logical one to WDCE and WDE */
//         /* Keep old prescaler to prevent unintentional time-out */
//         WDTCSR |= (1<<WDCE) | (1<<WDE);
//         /* Turn off WDT */
//         WDTCSR = 0x00;
//         __enable_interrupt();
// }

// inline void wdt_disable(){
// void wdt_init() {
//     uint8_t origSREG = SREG;
//     cli();
//     wdt_reset();
// //     __asm__ ( "cli" );
//     MCUSR &= ~(_BV(WDRF));
//     WDTCSR |= _BV(_WD_CHANGE_BIT) | _BV(WDE);
//     WDTCSR = 0;
//     SREG = origSREG;
//     sei();
// }

// // blocking call to serialRead
// uint8_t readByte(){
//   while(!serialAvailable());
//   return serialRead();
// }

void error(uint8_t code){
  signal.setSignal(code);
}

// void setmode(uint8_t mode){
//   eeprom_write_byte(EEPROM_MODE_ADDRESS, mode);
// }

// void reset(uint8_t mode){
//   eeprom_write_byte(EEPROM_MODE_ADDRESS, mode);
//   wdt_enable(WDTO_120MS); // will 30ms or 60ms work with the bootloader?
//   for(;;);
// }

// hack/workaround to get round problem with virtual constructors/methods:
// undefined reference to `__cxa_pure_virtual'
// extern "C" {
//   void __cxa_pure_virtual(){}
//   // todo: error handling within function
// }
