import rwmidi.*;

rwmidi.MidiOutput output;
ProcessingTaquito taquito;

void setup() {
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

public void release(Position pos){
  if(note != -1)
    output.sendNoteOff(channel, note, 0);
  note = -1;
  output.sendController(channel, breathCC, 0);
}

public void position(Position pos){
  println(pos.toString());
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
  delay(100);
}








