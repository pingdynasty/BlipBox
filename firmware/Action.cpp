#include "Action.h"
#include "globals.h"
#include "serial.h"
#include "defs.h"
#include "macros.h"
#ifdef BLIPBOX_CV4
#include "ControlVoltageAction.h"
#endif /* BLIPBOX_CV4 */
#include <stddef.h>

void SelectPresetAction::on(float data){
  if(data == 1.0f)
    blipbox.loadPreset(getPresetIndex());
}

float SelectPresetAction::getValue() { 
  return getPresetIndex() == blipbox.getPresetIndex() ? 1.0f : 0.0f;
}

int16_t MidiAction::constrain(int16_t value){
  return min(max(value, minimum*0x7f), maximum*0x7f);
}

int8_t MidiAction::constrain(int8_t value){
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

// static method
Action* Action::createAction(uint8_t type){
  Action* action = NULL;
  switch(type & MIDI_STATUS_MASK){
#ifdef BLIPBOX_CV4
  case CONTROL_VOLTAGE_ACTION_TYPE:
    action = new ControlVoltageAction();
    break;
#endif /* BLIPBOX_CV4 */
  case MIDI_NRPN_ACTION_TYPE:
    action = new MidiNRPNAction();
    break;
  case MIDI_NOTE_RANGE_ACTION_TYPE:
    action = new MidiNoteRangeAction();
    break;
  case SELECT_PRESET_ACTION_TYPE:
    action = new SelectPresetAction();
    break;
  case MIDI_AFTERTOUCH:
    action = new MidiAftertouchAction();
    break;
  case MIDI_CONTROL_CHANGE:
    action = new MidiControllerAction();
    break;
  case MIDI_NOTE_ON:
  case MIDI_NOTE_OFF:
      action = new MidiNoteAction();
    break;
  case MIDI_PROGRAM_CHANGE:
      action = new MidiProgramChangeAction();
    break;
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
