#include "Action.h"
#include "ControlVoltageAction.h"
#include "serial.h"
#include "macros.h"
#include <stddef.h>

int16_t AbstractAction::constrain(int16_t value){
  return min(max(value, minimum*0x7f), maximum*0x7f);
}

int8_t AbstractAction::constrain(int8_t value){
  return min(max(value, minimum), maximum);
}

void MidiAction::sendMessage(uint8_t data1, int8_t data2){
  serialWrite(status);
  serialWrite(data1 & 0x7f);
  serialWrite(data2 & 0x7f);
}

void MidiAction::sendMessage(int16_t data){
  serialWrite(status);
  serialWrite(data & 0x7f);
  serialWrite((data>>7) & 0x7f);
}

void MidiAction::sendMessage(int8_t data){
  serialWrite(status);
  serialWrite(data & 0x7f);
}

Action* Action::createAction(uint8_t type){
  Action* action = NULL;
  switch(type & MIDI_STATUS_MASK){
  case CONTROL_VOLTAGE_ACTION_TYPE:
    action = new ControlVoltageAction();
    break;
  case MIDI_AFTERTOUCH:
  case MIDI_CONTROL_CHANGE:
    action = new MidiControllerAction();
    break;
  case MIDI_NOTE_ON:
  case MIDI_NOTE_OFF:
      action = new MidiNoteAction();
    break;
//   case MIDI_PROGRAM_CHANGE:
//     break;
  case MIDI_CHANNEL_PRESSURE:
    action = new MidiChannelPressureAction();
    break;
  case MIDI_PITCH_BEND:
    action = new MidiPitchBendAction();
    break;
//   case MIDI_SYSTEM_MESSAGE:
//     break;
//   default:
//     action = new Action();
  }
//   }
  return action;
}
