#include "Action.h"
#include <stddef.h>

Action* Action::createAction(uint8_t type){
  Action* action = NULL;
  switch(type & MIDI_STATUS_MASK){
  case MIDI_AFTERTOUCH:
  case MIDI_CONTROL_CHANGE:
    action = new MidiControllerAction();
    break;
//   case MIDI_NOTE_ON:
//   case MIDI_NOTE_OFF:
//       action = new MidiNoteAction();
//     break;
//   case MIDI_PROGRAM_CHANGE:
//     break;
//   case MIDI_CHANNEL_PRESSURE:
//     break;
//   case MIDI_PITCH_BEND:
//       action = new MidiPitchBendAction();
//     break;
//   case MIDI_SYSTEM_MESSAGE:
//     break;
//   default:
//     action = new Action();
  }
//   }
  return action;
}
