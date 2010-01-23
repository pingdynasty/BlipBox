#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#define SEQUENCER_STEPS 8
#define MODULATION_CC 1

class Step {
 public:
  Step() :
    note(60), duration(127), velocity(63)
/*     note(60), duration(63) */
  {}
//     note = 60;
// //     delay = 0;
//     duration = 127;
//     velocity = 127;
// //     modulation = 0;
//   }

  int8_t note;
  int8_t delay;
  int8_t duration;
  int8_t velocity;
//   int8_t modulation;
};

class Sequencer : public MidiInterface {

public:
  void init(MidiInterface* _player, MidiInterface* _controller){
    player = _player;
    controller = _controller;
    divisor = 24; // 24 MIDI clocks / quarter note.
    frequency = 512;
  }

  void stepOn(const Step& step){
//     player->noteOn(step.note, 0x7f);
    player->noteOn(step.note, step.velocity);
//     player->controlChange(MODULATION_CC, step.modulation);
  }

  void stepOff(const Step& step){
    player->noteOff(step.note, 0);
  }

  void midiTick(){
    if(internalClock && ++tick == frequency){
      midiClock();
      tick = 0;
    }
  }

  void midiClock(){
    // 24 pulses per quarter note
    if(playing){
      uint8_t tick = clock % divisor;
      if(tick == 0)
        controller->controlChange(clock/divisor+65, 0x7f);
      else if(tick == divisor-1)
        controller->controlChange(clock/divisor+65, 0);

      Step& step = steps[clock/divisor];
      uint8_t pos = (uint16_t)step.delay*divisor/127;
      if(tick == pos)
        stepOn(step);
      pos = (uint16_t)step.duration*divisor/127;
      // note that step end point is not affected by changes to its start point
      if(tick == pos)
        stepOff(step);
      if(++clock / divisor >= SEQUENCER_STEPS)
        clock = 0;
    }
  }

  void startSong(){
    clock = 0;
    playing = true;
    player->startSong();
    controller->controlChange(81, 0x7f);
  }

  void stopSong(){
    playing = false;
    player->stopSong();
    controller->controlChange(81, 0);
  }

  void continueSong(){
    playing = true;
    player->continueSong();
    controller->controlChange(81, 0x7f);
  }

  void allNotesOff(){
    player->allNotesOff();
  }

  void noteOff(int note, int velocity){
    player->noteOff(note, velocity);
  }

  void noteOn(int note, int velocity){
    player->noteOn(note, velocity);
  }

  void controlChange(int cc, int value){
    // cc mappings:
    // 1-8 : set note
    // 9-16 : set delay
    // 17-24 : set duration
    // 25-32 : velocity
    // 33-40 : modulation
    // 41-48 : modulation
    // 49-56 : modulation
    // buttons
    // 65-72 step position 1-8
    // 73-80 step position 9-16
    // start         81
    // stop          82
    // continue      83
    // panic         84
    if(cc < 40){
      --cc; // adjust to 0-7 group
      Step& step = steps[cc%SEQUENCER_STEPS];
      switch(cc/SEQUENCER_STEPS){
      case 0:
        step.note = value;
        break;
      case 1:
        step.delay = value;
        break;
      case 2:
        step.duration = value;
        break;
      case 3:
        step.velocity = value;
        break;
//     case 4:
//       step.modulation = value;
//       break;
      }
    }else if(cc == 55){
      divisor = ++value;
    }else if(cc == 56){
      frequency = ++value << 7; // range from 128 to 16384
    }else if(cc > 80 && cc < 85){
      switch(cc){
      case 81:
        if(playing)
          stopSong();
        else if(value)
          startSong();
        break;
      case 82:
        if(playing && value)
          stopSong();
        else if(value)
          continueSong();
        break;
      case 83:
        internalClock = value;
        break;
      case 84:
        allNotesOff();
        break;
      }
    }else{
      player->controlChange(cc, value);
    }
  }

private:
  Step steps[SEQUENCER_STEPS];
  uint8_t length;
  MidiInterface* player;
  MidiInterface* controller;
  uint8_t divisor;
  uint8_t clock;
  boolean playing;

  uint16_t tick;
  uint16_t frequency;
  boolean internalClock;  
};

#endif /* _SEQUENCER_H_ */
