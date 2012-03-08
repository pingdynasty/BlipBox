#include <stdlib.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "defs.h"
#include "device.h"
#include "globals.h"
#include "BlipBox.h"
#include "Parameters.h"
#include "PresetChooser.h"
#include "SerialProtocol.h"
// #include "MidiSerialReader.h"
#include "MidiMessageReader.h"
#include "SerialProtocolReader.h"
#include "PresetSerialiser.h"
#ifdef BLIPBOX_CV4
#include "spi.h"
#endif /* BLIPBOX_CV4 */

class EventHandlerWrapper : public EventHandler {
private:
  EventHandler* handler;
public:
  EventHandlerWrapper(EventHandler* h) : handler(h) {}
  void handle(TouchEvent& event){
    handler->handle(event);
  }
  void handle(MidiEvent& event){
    handler->handle(event);
  }
};

void setup() {
  blipbox.config.init();
  blipbox.init();
  beginSerial(blipbox.config.serialSpeed);
  blipbox.animator = NULL;
//   blipbox.animator = new GreetingAnimator();
  blipbox.message(ALERT);
}

void loop() {
  blipbox.loop();
}

void BlipBox::init() {
  cli(); // disable interrupts
  receiver = new SerialProtocolReader();
  eventhandler = new DefaultEventHandler();
  animator = NULL;
  keys.init();
  leds.init();
#ifdef BLIPBOX_CV4
  spi_init();
#endif /* BLIPBOX_CV4 */
  sei(); // enable interrupts
  loadPreset(config.preset);
}

void BlipBox::loop() {
  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    keys.keyscan(); // triggers eventhandler
    previousMillis = millis();   // remember the last time we did this
    // counter overflows at 65536
    signal.tick(++counter);
    if(animator != NULL)
      animator->tick(counter);
  }
}

void BlipBox::setSerialReader(SerialReader* handler){
  delete receiver;
  receiver = handler;
}

void BlipBox::resetSerialReader(){
  delete receiver;
  receiver = new SerialProtocolReader();
}

void BlipBox::setEventHandler(EventHandler* handler){
  delete eventhandler;
  eventhandler = handler;
}

void BlipBox::resetEventHandler(){
  delete eventhandler;
  eventhandler = new DefaultEventHandler();
}

void BlipBox::setMode(BlipBoxMode mode){
  leds.clear();
  switch(mode){
  case EDIT_MODE: {
    PresetChooser* chooser = new PresetChooser();
    animator = chooser;
    setSerialReader(new SerialReader());
    setEventHandler(new EventHandlerWrapper(chooser));
    break;
  }
  case MIDI_MODE: {
    beginSerial(MIDI_SERIAL_SPEED);
    animator = &preset;
    setSerialReader(new MidiMessageReader(&preset));
    setEventHandler(new EventHandlerWrapper(&preset));
    break;
  }
  case BLIP_MODE: {
    beginSerial(blipbox.config.serialSpeed);
    animator = NULL;
    resetSerialReader();
    resetEventHandler();
//     ^ same as: setSerialReader(new SerialProtocolReader());
    break;
  }
  }
}

void BlipBox::message(MessageType code){
  signal.setSignal(code);
}

// returns current preset 0-8, where 0 is BLIP mode.
uint8_t BlipBox::getPresetIndex(){
  return config.preset;
}

// index should be 0-8, where 0 denotes BLIP mode, USB connection
// and 1-8 denotes MIDI mode
void BlipBox::loadPreset(uint8_t index){
  config.preset = index;
  if(index){
    preset.load(index-1);
    setMode(MIDI_MODE);
  }else{
    setMode(BLIP_MODE);
  }
}

class PresetReceiver : public PresetLoader {
private:
  uint8_t index;
public:
  PresetReceiver(uint8_t i) : index(i) {}
  void done(Preset* preset){
    preset->save(index);
    close();
  }
  void fail(){
    blipbox.message(MESSAGE_READ_ERROR);
    close();
  }
  void close(){
    blipbox.leds.clear();
    blipbox.leds.flip();
    blipbox.animator = NULL;
    blipbox.resetSerialReader();
  }
};

// preset here should be 0-7
void BlipBox::receivePreset(uint8_t index){
//   PresetSerializer serialiser = new PresetSerializer();
//   setSerialReader(serialiser);
//   animator = serialiser;
  PresetLoader* loader = new PresetReceiver(index);
  setSerialReader(loader);
  animator = loader;
//   sei(); // re-enable interrupts to capture more serial data
//   leds.clear();
//   leds.flip();
//   if(loader->read(&preset))
//     preset.save(index);
//   else
//     message(MESSAGE_READ_ERROR);
//   animator = NULL;
//   resetSerialReader();
}

class PresetSender : public PresetSerialiser {
  void writeBlock(uint8_t* data, uint8_t size){
    for(int i=0; i<size; ++i)
      sendParameterMessage(MIDI_ZONE_PARAMETER_ID, data[i]);
  }
};

// preset here should be 0-7
void BlipBox::sendPreset(uint8_t index){
  preset.load(index);
  sendParameterMessage(MIDI_ZONE_PARAMETER_ID, index);
  PresetSender sender;
  sender.write(&preset);
  preset.load(config.preset);
}

void BlipBox::sendParameter(uint8_t pid){
  sendParameterMessage(pid, getParameter(pid));
}

void BlipBox::sendConfigurationParameters(){
  message(ALERT);
  sendParameter(SENSITIVITY_PARAMETER_ID);
  sendParameter(BRIGHTNESS_PARAMETER_ID);
  sendParameter(SERIAL_SPEED_PARAMETER_ID);
  sendParameter(PRESET_PARAMETER_ID);
  sendParameter(VERSION_PARAMETER_ID);
}

#ifdef BLIPBOX_CV4
// control voltages are 12 bit integers, channel is 0-3
// ch	cs  ab
// 0	0   0
// 1	1   0
// 2	0   1
// 3	1   1
uint16_t BlipBox::getControlVoltage(uint8_t channel){
  return controlvoltages[channel];
}
void BlipBox::setControlVoltage(uint8_t channel, uint16_t value){
  if(controlvoltages[channel] != value){
    controlvoltages[channel] = value;
    // send value to dac
    // first bit of channel is 0 for channels 0 and 2, 1 for channels 1 and 3
    spi_cs(channel & 0x01); 
    uint8_t flags = DAC_SHDN_BIT;
    // second bit of channel is 0 for channels 0 and 1, 1 for channels 2 and 3
    if(channel & 0x02)
      flags |= DAC_A_B_BIT;
    spi_send(flags | ((value >> 8) & 0x0f));
    spi_send(value & 0xff);
  }
}
#endif /* BLIPBOX_CV4 */

// Interrupt routines

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
ISR(USART_RX_vect){
  unsigned char c = UDR0;
#else
ISR(SIG_UART_RECV){
  unsigned char c = UDR;
#endif
  blipbox.receiver->serialInput(c);
}

ISR(TIMER1_OVF_vect){
  blipbox.leds.displayCurrentRow();
}
