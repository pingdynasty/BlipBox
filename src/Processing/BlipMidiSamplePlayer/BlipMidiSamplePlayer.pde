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
int range = 10;
int cc1 = 18;
int cc2 = 19;
int channel = 0;
int cc1value = 64;
int cc2value = 64;

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

private int normalize(int difference, int value){
  if(difference > 10){
    if(++value > 127)
      value = 127;
  }
  else if(difference < -10){
    if(--value < 0)
      value = 0;
  }
  return value;
}

public void drag(int x, int y, int dx, int dy){
  int value = normalize(x - dx, cc1value);
  if(value != cc1value){
    cc1value = value;
    output.sendController(channel, cc1, cc1value);
  }
  value = normalize(x - dx, cc2value);
  if(value != cc2value){
    cc2value = value;
    output.sendController(channel, cc2, cc2value);
  }
}

void draw() {
  blipbox.fade();
  delay(100);
}








