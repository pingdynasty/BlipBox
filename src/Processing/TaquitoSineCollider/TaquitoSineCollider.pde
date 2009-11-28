import oscP5.*;
import netP5.*;
import supercollider.*;

Synth synth;
ProcessingTaquito taquito;

void setup() {
  size(800, 200);
  synth = new Synth("sine");
  synth.set("amp", 0.0);
  // create synth
  synth.create();
  taquito = new ProcessingTaquito(this, 0);
}

int freq;
float amp;
int freqmin = 440;
int freqmax = 880; // 3520;
float ymin = 270;
float ymax = 880;

public void release(Position pos){
  println("release "+pos);
  amp = 0.0;
  synth.set("amp", amp); 
}

public void position(Position pos){
  println(pos.toString());
  freq = pos.getX(440, freqmax+1);
  synth.set("freq", freq);
  amp = (pos.getY() - ymin) / (ymax - ymin);
  synth.set("amp", amp);
}

void draw (){
  background(0);
  stroke(255);
  int x = (freq - freqmin)*width/(freqmax - freqmin);
  int y = height - (int)(amp*height);
  line(x, 0, x, height);
  line(0, y, width, y);
}

void stop (){
  synth.free();
}


