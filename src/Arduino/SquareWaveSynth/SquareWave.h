#ifndef _SQUAREWAVE_H_
#define _SQUAREWAVE_H_

static uint16_t prescalers[] = {1, 8, 32, 64, 128, 256, 1024};

class SquareWave {
private:

public:
  void init(){
    pinMode(3, OUTPUT);
    pinMode(11, OUTPUT);
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20);
    setDutyCycle(0.5);
  }

  /* accepts values 1-7 to set prescalers 1/8/32/64/128/256/1024,
     0 turns timer off */
  void setPrescaler(uint8_t divisor){
    if(divisor > 7)
      return;
    TCCR2B = _BV(WGM22) | divisor;
  }

  /* expects a value 0.0-1.0 */
  void setDutyCycle(float value){
    OCR2B = (uint8_t)(OCR2A * value);
  }

  void setTopLimit(uint8_t limit){
    OCR2A = limit;
  }

  uint8_t getPrescaler(float freq){
    float val = 16000000 / 4 / 255 / freq;
    for(int i=0; i<7; ++i)
      if(prescalers[i] > val)
	return i+1;
    return 0;
  }

  uint8_t getLimit(float freq, uint8_t pre){
    return round(16000000 / 4 / prescalers[pre-1] / freq);
  }

  void setFrequency(float freq){
    uint8_t pre = getPrescaler(freq);
    uint8_t limit = getLimit(freq, pre);
    setPrescaler(pre);
    setTopLimit(limit);
  }

  float midiToFreq(uint8_t note){
    return 440.0 * pow(2, (note - 69) / 12.0); 
  }

  void setMidiNote(uint8_t note){
    setFrequency(midiToFreq(note));
  }
};

#endif /* _SQUAREWAVE_H_ */
