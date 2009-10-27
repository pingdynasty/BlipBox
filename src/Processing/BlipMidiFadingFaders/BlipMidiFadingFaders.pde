import rwmidi.*;

rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int speed = 100;
int channel = 0;

int[] cctable = new int[127];
Fader[] faders = new Fader[10];

void setup() {
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);

  for(int i=0; i<faders.length; ++i)
    setFader(i+20, i);
}

public void drawFaders(){
  for(int i=0; i<faders.length; ++i)
    faders[i].draw();
}

public void setFader(int cc, int position){
  cctable[cc] = position;
  faders[position] = new Fader(cc, position);
}

public Fader getFader(int cc){
  return faders[cctable[cc]];
}

class Fader {
  int cc;
  int position;
  int value;

  public Fader(int cc, int position){
    this.cc = cc;
    this.position = position;
  }

  public void update(int value){
    if(value > 0 && value < 128 && this.value != value){
      this.value = value;
      draw();
      println("updated "+this);
    }
  }

  final int step = 127 / 8;
  public void draw(){
    //      blipbox.setLedColumn(position, 0);
    int i = 0;
    while(i < value/step && i < 8)
      blipbox.setLed(position, i++, 0xff);
    blipbox.setLed(position, i, value % step);
  }
  
  public String toString(){
    return position+"/"+cc+":"+value;
  }
}

public void controllerChangeReceived(Controller cc){
  println("received "+cc);
  Fader fader = getFader(cc.getCC());
  if(fader != null)
    fader.update(cc.getValue());
}

public void position(int x, int y){
  int col = blipbox.getX(0, 10);
  Fader fader = faders[col];
  fader.update(blipbox.getY(127, 0));
  output.sendController(channel, fader.cc, fader.value);
}

void draw() {
  blipbox.fade();
  drawFaders();
  delay(speed);
}


