#ifndef _MIDICONSTANTS_H_
#define _MIDICONSTANTS_H_

#define NRPN_PARAMETER_MSB_CC         99
#define NRPN_PARAMETER_LSB_CC         98
#define NRPN_VALUE_MSB_CC             6
#define NRPN_VALUE_LSB_CC             38

#define MIDI_STATUS_MASK              0xf0
#define MIDI_CHANNEL_MASK             0x0f
#define MIDI_VALUE_MASK               0x7f

#define MIDI_NOTE_OFF                 0x80
#define MIDI_NOTE_ON                  0x90
#define MIDI_AFTERTOUCH               0xA0
#define MIDI_CONTROL_CHANGE           0xB0
#define MIDI_PROGRAM_CHANGE           0xC0
#define MIDI_CHANNEL_PRESSURE         0xD0
#define MIDI_PITCH_BEND               0xE0
#define MIDI_SYSTEM_MESSAGE           0xF0

#define MIDI_SYSEX_START              0xF0
#define MIDI_TIME_CODE                0xF1
#define MIDI_SONG_POSITION            0xF2
#define MIDI_SONG_SELECT              0xF3
#define MIDI_TUNE_REQUEST             0xF6
#define MIDI_SYSEX_END                0xF7
#define MIDI_TIMING_CLOCK             0xF8
#define MIDI_TICK                     0xF9
#define MIDI_START_SONG               0xFA
#define MIDI_CONTINUE_SONG            0xFB
#define MIDI_STOP_SONG                0xFC
#define MIDI_ACTIVE_SENSING           0xFE
#define MIDI_SYSTEM_RESET             0xFF

#define MIDI_CC_BANK_SELECT 0
#define MIDI_CC_MOD_WHEEL   1
#define MIDI_CC_BREATH      2
#define MIDI_CC_FOOT_PEDAL  4
#define MIDI_CC_PORTAMENTO  5
#define MIDI_CC_VOLUME      7
#define MIDI_CC_BALANCE     8
#define MIDI_CC_PAN        10
#define MIDI_CC_EXPRESSION 11
#define MIDI_CC_EFFECT1    12
#define MIDI_CC_EFFECT2    13

#endif /* _MIDICONSTANTS_H_ */
