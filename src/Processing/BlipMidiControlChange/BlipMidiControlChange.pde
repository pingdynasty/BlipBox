import rwmidi.*;

rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

void setup() {
  println("MIDI output: "+RWMidi.getOutputDevices()[0].getName());
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
}

int channel = 0;
int cc1 = 70;
int cc2 = 71;

public void position(int x, int y){
  output.sendController(channel, cc1, blipbox.getX(0, 127));
  output.sendController(channel, cc2, blipbox.getY(0, 127));
  blipbox.setLed(blipbox.getX(0, 10), blipbox.getY(8, 0), 0xff);
}

void draw() {
  blipbox.fade();
  delay(100);
}








