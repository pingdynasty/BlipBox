import oscP5.*;
import netP5.*;
import supercollider.*;

Synth synth;
ProcessingTaquito taquito;

/* SuperCollider
SynthDef(\sine, { |amp = 0.5, freq = 440|
	var data = SinOsc.ar(freq, 0, amp);
	Out.ar(0, data ! 2);
}).store;
*/
void setup() {
  size(800, 200);
  synth = new Synth("sine");
  synth.set("amp", 0.0);
  // create synth
  synth.create();
  taquito = new ProcessingTaquito(this, 0);
}

int freqmin = 440;
int freqmax = 880; // 3520;

public void release(Position pos){
  println("release "+pos);
  synth.set("amp", 0.0); 
}

public void position(Position pos){
  println(pos.toString());
  synth.set("freq", pos.getX(freqmin, freqmax+1));
  synth.set("amp", pos.getY());
}

void draw (){
  background(0);
  stroke(255);
  if(taquito.isScreenPressed()){
    int x = taquito.getX(0, width);
    int y = height - taquito.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}

void stop (){
  synth.free();
}



