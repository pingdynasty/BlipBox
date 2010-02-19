import rwmidi.*;
import controlP5.*;

rwmidi.MidiOutput output;
ProcessingTaquito taquito;
ControlP5 controlP5;

void setup() {
  size(600, 400);
  controlP5 = new ControlP5(this);
  controlP5.addSlider("root", 0, 127, root, 80, 50, 10, 100).setId(1);
  controlP5.addSlider("range", 0, 127, range, 120, 50, 10, 100).setId(2);
  output = RWMidi.getOutputDevices()[0].createOutput();
  taquito = new ProcessingTaquito(this, 0);
}

// y / breath: 270 - 842
// x / pos   : 0/40 - 1024
// x min creeps up to 50 when max breath is applied.

int note = -1;
int root = 60;
int range = 13;
int channel = 0;
int breathCC = 1;

void range(int range){
  println("set range "+range);
  this.range = range;
}

void root(int root){
  println("set root "+root);
  this.root = root;
}

public void release(Position pos){
  if(note != -1)
    output.sendNoteOff(channel, note, 0);
  note = -1;
  output.sendController(channel, breathCC, 0);
}

public void position(Position pos){
//  println(pos.toString());
  int velocity = pos.getY(1, 128);
  int newnote = pos.getX(root, root+range);
  if(note != newnote){
    if(note != -1)
      output.sendNoteOff(channel, note, 0);
    note = newnote;
    output.sendNoteOn(channel, note, velocity);
  }
  output.sendController(channel, breathCC, velocity);
}

void draw() {
  background(0xeee);
  stroke(255);
  if(taquito.isScreenPressed()){
    int x = taquito.getX(0, width);
    int y = height - taquito.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}








