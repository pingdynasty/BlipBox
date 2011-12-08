#ifndef _EVENT_H_
#define _EVENT_H_

#include "Position.h"
#include "MidiConstants.h"
#include <stdlib.h>

#define MIDI_EVENT_MASK         0x80
#define TOUCH_EVENT_MASK        0x01
#define RELEASE_EVENT_TYPE      0x01
#define PRESS_EVENT_TYPE        0x03
#define TAP_EVENT_TYPE          0x05
#define TAPTAP_EVENT_TYPE       0x07
#define DRAG_EVENT_TYPE         0x09

class Event {
public:
  uint8_t type;
  Event() {}
/*   Event(Position* apos) */
/*     : pos(apos) {} */
  Event(uint8_t atype)
    : type(atype) {}
  uint8_t getType(){
    return type;
  }
  bool isMidi(){
    return type & MIDI_EVENT_MASK;
  }
  bool isTouch(){
    return type & TOUCH_EVENT_MASK;
  }
  bool isPress(){
    return type == PRESS_EVENT_TYPE;
  }
  bool isTap(){
    return type == TAP_EVENT_TYPE;
  }
  bool isTapTap(){
    return type == TAPTAP_EVENT_TYPE;
  }
  bool isDrag(){
    return type == DRAG_EVENT_TYPE;
  }
  bool isRelease(){
    return type == RELEASE_EVENT_TYPE;
  }
};

class TouchEvent : public Event {
public:
  Position* pos;
//   TouchEvent(uint8_t type, Position* apos) : Event(type), pos(apos) {}
  Position* getPosition(){
    return pos;
  }
};

class MidiEvent : public Event {
private:
  uint8_t data1;
  uint8_t data2;
public:
  MidiEvent(uint8_t type, uint8_t d1, uint8_t d2) : Event(type), data1(d1), data2(d2) {}
  bool isNoteOn(){
    return (type & MIDI_STATUS_MASK) == MIDI_NOTE_ON;
  }
  bool isNoteOff(){
    return (type & MIDI_STATUS_MASK) == MIDI_NOTE_OFF;
  }
  bool isControlChange(){
    return (type & MIDI_STATUS_MASK) == MIDI_CONTROL_CHANGE;
  }
  bool isPitchBend(){
    return (type & MIDI_STATUS_MASK) == MIDI_PITCH_BEND;
  }  
  uint8_t getStatus(){
    return type & MIDI_STATUS_MASK;
  }
  uint8_t getChannel(){
    return type & MIDI_CHANNEL_MASK;
  }
  uint16_t getData(){
    return (data2<<7) & data1;
  }
  uint8_t getData1(){
    return data1;
  }
  uint8_t getData2(){
    return data2;
  }
};

/* enum EventType { */
/*   RELEASE, TAP, TAPTAP, DRAG */
/* }; */

#endif /* _EVENT_H_ */
