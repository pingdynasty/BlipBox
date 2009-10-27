import rwmidi.*;

rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

void setup() {
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
}

int tone;
int tonerange = 20;
int root = 60;
int channel = 1;
int cc1 = 18;
int cc2 = 19;

public void position(int x, int y){
  int newtone = blipbox.getX(root, root+tonerange);
  if(newtone != tone){
    if(tone != -1) // release last note
      output.sendNoteOff(1, tone, 0);
    output.sendNoteOn(channel, newtone, blipbox.getY(0, 127));
    tone = newtone;
  }
  blipbox.setLed(blipbox.getX(0, 10), blipbox.getY(8, 0), 0xff);
}

public void release(int x, int y){
  output.sendNoteOff(channel, tone, 0);
  tone = -1;
}

public void taptap(int x, int y){
  output.sendController(channel, cc1, blipbox.getX(0, 127));
  output.sendController(channel, cc2, blipbox.getY(0, 127));
}

void draw() {
  blipbox.fade();
  delay(100);
}





