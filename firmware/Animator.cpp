#include "Animator.h"
#include "globals.h"

void FadeAnimator::tick(uint16_t counter){
  if(counter % prescaler == 0)
    blipbox.leds.sub(1);
}

void SignalAnimator::tick(uint16_t counter){
  // blink all leds
  if(signals && counter % 0xfff == 0){
      blipbox.leds.toggle();
      --signals;
  }
}

void DotAnimator::tick(uint16_t counter){
  if(counter % 32 == 0)
    blipbox.leds.sub(1);
  if(blipbox.keys.isPressed())
    blipbox.leds.setLed(blipbox.keys.getColumn(), blipbox.keys.getRow(), blipbox.config.brightness);
}

void CrossAnimator::tick(uint16_t counter){
  blipbox.leds.sub(1);
  if(blipbox.keys.isPressed())
    blipbox.display.setCross(blipbox.keys.getColumn(), blipbox.keys.getRow(), blipbox.config.brightness);
}

void CrissAnimator::tick(uint16_t counter){
  blipbox.leds.sub(1);
  if(blipbox.keys.isPressed())
    blipbox.display.setDiagonalCross(blipbox.keys.getColumn(), blipbox.keys.getRow(), blipbox.config.brightness);
}

void StarAnimator::tick(uint16_t counter){
  if(counter % 16 == 0)
    blipbox.leds.sub(1);
  if(blipbox.keys.isPressed())
    blipbox.display.setStar(blipbox.keys.getColumn(), blipbox.keys.getRow(), blipbox.config.brightness);
}

void ToggleAnimator::tick(uint16_t counter){
  if(counter % 0xff == 0)
    blipbox.leds.sub(1);
  if(blipbox.keys.isPressed()){
    blipbox.leds.toggle(blipbox.keys.getColumn(), blipbox.keys.getRow());
//     for(int x=blipbox.keys.getColumn()-1; x<blipbox.keys.getColumn()+2; ++x)
//       for(int y=blipbox.keys.getRow()-1; y<blipbox.keys.getRow()+2; ++y)
// 	blipbox.leds.toggle(x, y);
  }
}
