#include "defs.h"
#include "device.h"
#include "globals.h"

#include "MessageDispatcher.h"
#include "BlipBox.h"
#include "Parameters.h"

#include "MidiZoneEventHandler.h"

unsigned long previousMillis;        // will store last time write was done
uint16_t counter;

Animator* animator;
// #define NOF_ANIMATORS 5
// Animator animators[NOF_ANIMATORS] = {
// DotAnimator(), CrossAnimator(), 
// CrissAnimator(), ToggleAnimator(), StarAnimator() };
DotAnimator dot;
CrossAnimator cross;
CrissAnimator criss;
ToggleAnimator toggle;
StarAnimator star;

void setup() {
  blipbox.config.init();
  blipbox.init();
  blipbox.leds.start();
  blipbox.setMidiMode(false);
  blipbox.message(ALERT);

// timer0 (used for millis() counting)
//   TCCR0B = CS01 | CS00; // prescaler 64
//   TCCR0B = CS02;        // prescaler 256
//   TCCR0B = CS02 | CS00; // prescaler 1024
// 16MHz / 256 / 64 = 976.5625Hz
// 16MHz / 256 / 256 = 244.140625Hz
// 16MHz / 256 / 1024 = 61.03515625Hz
}

// volatile unsigned long timer0_overflow_count;
// SIGNAL(SIG_OVERFLOW0){
//   timer0_overflow_count++;
// }
// unsigned long millis(){
// 	return timer0_overflow_count * 64UL * 2UL / (F_CPU / 128000UL);
// }

void loop() {
  blipbox.tick();
//   readSensors();
  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    blipbox.sender.send();
    previousMillis = millis();   // remember the last time we did this
    // counter overflows at 65536
  }
  blipbox.signal.tick(++counter);
  if(animator != NULL)
    animator->tick(counter);
}

#define TAP_THRESHOLD 300 // ms between press and release to be considered tap
#define TAPTAP_THRESHOLD 600 // ms between taps to be considered double-tap

unsigned long lastpressed = 0xffff;
unsigned long lasttapped  = 0xffff;

void BlipBox::setSerialReader(SerialReader* handler){
  SerialReader* p = receiver;
  receiver = handler;
  if(p != &defaultreceiver && p != receiver)
    free(p);
}

void BlipBox::resetSerialReader(){
  SerialReader* p = receiver;
  receiver = &defaultreceiver;
  if(p != &defaultreceiver)
    free(p);
}

void BlipBox::setEventHandler(EventHandler* handler){
  EventHandler* p = eventhandler;
  eventhandler = handler;
  if(p != &defaulthandler && p != &midizones && p != handler)
    free(p);
  eventhandler->init();
}

void BlipBox::resetEventHandler(){
  EventHandler* p = eventhandler;
  eventhandler = &defaulthandler;
  if(p != &defaulthandler && p != &midizones)
    free(p);
}

void BlipBox::setMidiMode(bool midi){
  if(midi){
    eventhandler = &midizones;
    beginSerial(31250L);
    setSerialReader(new SerialReader());
    animator = &midizones;
  }else{
    eventhandler = &defaulthandler;
    beginSerial(blipbox.config.serialSpeed);
    blipbox.setFollowMode(0);
  }
}

void BlipBox::setEditMode(bool edit){
  blipbox.leds.clear();
  if(edit){
    PresetChooser* presetshandler = new PresetChooser();
    setEventHandler(presetshandler);
    setSerialReader(new SerialReader());
    animator = presetshandler;
  }else{
    resetEventHandler();
    resetSerialReader();
    blipbox.setFollowMode(0);
  }
}

void BlipBox::tick(){
  switch(keys.keyscan()){
  case DISENGAGED:
    break;
  case RELEASED:
    eventhandler->release(keys.getPosition());
    if(millis() - lasttapped < TAPTAP_THRESHOLD){
      eventhandler->taptap(keys.getPosition());
    }else if(millis() - lastpressed < TAP_THRESHOLD){
      eventhandler->tap(keys.getPosition());
      lasttapped = millis();
    }
    break;
  case PRESSED:
    eventhandler->press(keys.getPosition());
    lastpressed = millis();
    break;
  case DRAGGED:
  case UNCHANGED: // same column/row, possibly different x/y
    eventhandler->drag(keys.getPosition());
  }
}

void BlipBox::message(MessageType code){
  signal.setSignal(code);
}

void BlipBox::init() {
  cli(); // disable interrupts
  keys.init();
  leds.init();
  sender.init();
  setFollowMode(config.followMode);
  receiver = &defaultreceiver;
  sei(); // enable interrupts
}

void BlipBox::setFollowMode(uint8_t mode) {
  config.followMode = mode;
  switch(config.followMode){
  case 1:
    animator = &dot;
    break;
  case 2:
    animator = &cross;
    break;
  case 3:
    animator = &criss;
    break;
  case 4:
    animator = &toggle;
    break;
  case 5:
    animator = &star;
    break;
  default:
    animator = NULL;
  }
}

void sendParameter(uint8_t pid){
  blipbox.sender.parameter.update(pid, getParameter(pid));
  blipbox.sender.parameter.send();
}

void BlipBox::sendConfigurationParameters(){
  message(ALERT);
  sendParameter(SENSITIVITY_PARAMETER_ID);
  sendParameter(BRIGHTNESS_PARAMETER_ID);
  sendParameter(TLC_GSCLK_PERIOD_PARAMETER_ID);
  sendParameter(SERIAL_SPEED_PARAMETER_ID);
  sendParameter(FOLLOW_MODE_PARAMETER_ID);
}

void BlipBox::sendMidiZones(){
  blipbox.sender.parameter.update(MIDI_ZONE_PARAMETER_ID, midizones.getPreset());
  blipbox.sender.parameter.send();
  uint8_t data[4];
  for(int i=0; i<8; ++i){
    midizones.getZone(i).write(data);
    for(int j=0; j<4; ++j){
      blipbox.sender.parameter.update(MIDI_ZONE_PARAMETER_ID, data[j]);
      blipbox.sender.parameter.send();
    }
  }
  message(ALERT);
}

// Interrupt routines 

#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV){
  unsigned char c = UDR0;
#else
SIGNAL(SIG_UART_RECV){
  unsigned char c = UDR;
#endif
  blipbox.receiver->serialInput(c);
}

ISR(TIMER1_OVF_vect){
  blipbox.leds.displayCurrentRow();
}
