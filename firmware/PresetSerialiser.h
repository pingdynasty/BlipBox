#ifndef _PRESET_SERIALISER_H_
#define _PRESET_SERIALISER_H_

#include <inttypes.h>
#include "defs.h"
#include "serial.h"
#include "BufferedSerialReader.h"
#include "Animator.h"
#include "Preset.h"
#include "globals.h"

#define PRESET_SERIALISER_BUFFER_SIZE (MIDI_ZONE_PRESET_SIZE+1)
/**
   todo: it seems wrong that read requires 
   its own buffer as we are already read buffering...
*/
class PresetSerialiser  {
public:
  bool read(Preset* preset){
    uint8_t checksum;
    uint8_t buf[PRESET_SERIALISER_BUFFER_SIZE];
    readBlock(buf, 2);
    uint8_t zones = buf[0];
    uint8_t sz = buf[1];
    for(int i=0; i<zones; ++i){
      readBlock(buf, sz);
      checksum += preset->readZone(buf, i);
      sz = buf[sz-1];
    }
    if(sz == checksum){
      done(preset);
      return true;
    }else{
      fail();
      return false;
    }
  }
  void write(Preset* preset){
    uint8_t checksum = 0;
    uint8_t buf[PRESET_SERIALISER_BUFFER_SIZE];
    uint8_t size = preset->getNumberOfZones();
    buf[0] = size;
    writeBlock(buf, 1);
    for(int i=0; i<size; ++i){
      uint8_t sz = preset->writeZone(buf+1, i);
      buf[0] = sz;
      writeBlock(buf, sz+1);
      checksum += sz;
    }
    buf[0] = checksum;  
    writeBlock(buf, 1);
    done(preset);
  }
  virtual void writeBlock(uint8_t* data, uint8_t size){
  }
  virtual void readBlock(uint8_t* data, uint8_t size){
  }
  virtual void done(Preset* preset){
  }
  virtual void fail(){
  }
};

#define BUFFER_SIZE 32
class PresetLoader : public PresetSerialiser, public Animator, public BufferedSerialReader<BUFFER_SIZE> {
public:
  virtual void tick(uint16_t counter){
    // would be nice to show an animation here until the preset is actually loaded.
    // should have a timeout clause
    blipbox.leds.sub(1);
    blipbox.leds.setLed(serialAvailable()/8, serialAvailable()%8, 0xff);
    blipbox.leds.flip();
  }
  virtual void writeBlock(uint8_t* data, uint8_t size){
    for(uint8_t i=0; i<size; ++i)
      serialWrite(data[i]);
  }
  virtual void readBlock(uint8_t* data, uint8_t size){
    while(serialAvailable() < size); // wait
    for(uint8_t i=0; i<size; ++i)
      data[i] = serialRead();
  }  
};


#endif /* _PRESET_SERIALISER_H_ */
