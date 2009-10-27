import rwmidi.*;

rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

void setup() {
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
}

int note;
int velocity;
int root = 60;
int range = 20;
int channel = 0;

public void tap(int x, int y){
  note = blipbox.getX(root, root+range);
  output.sendNoteOn(channel, note, blipbox.getY(0, 127));
}
public void release(int x, int y){
  output.sendNoteOff(channel, note, 0);
}
public void position(int x, int y){
  blipbox.setLed(blipbox.getX(0, 10), blipbox.getY(8, 0), 0xff);
}

void draw() {
  blipbox.fade();
  delay(100);
}




