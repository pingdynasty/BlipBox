#ifndef _MIDICONSTANTS_H_
#define _MIDICONSTANTS_H_


#define NRPN_PARAMETER_MSB_CC         99
#define NRPN_PARAMETER_LSB_CC         98
#define NRPN_VALUE_MSB_CC             6
#define NRPN_VALUE_LSB_CC             38

#define MIDI_STATUS_MASK              0xf0
#define MIDI_CHANNEL_MASK             0x0f

#define MIDI_NOTE_OFF                 0x80
#define MIDI_NOTE_ON                  0x90
#define MIDI_AFTERTOUCH               0xA0
#define MIDI_CONTROL_CHANGE           0xB0
#define MIDI_PROGRAM_CHANGE           0xC0
#define MIDI_CHANNEL_PRESSURE         0xD0
#define MIDI_PITCH_BEND               0xE0
#define MIDI_SYSTEM_MESSAGE           0xF0

#endif /* _MIDICONSTANTS_H_ */
