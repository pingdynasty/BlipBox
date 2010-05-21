rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int brightness = 0xff;
int speed = 200;
int root = 60;
int channel = 0;
int velocity = 80;

int ship = 3;
int corner = 0;
int width = 7;

void setup(){
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
}

void release(Position pos){
  println("released");
}

void position(Position pos){
  if(pos.getY(8, 0) < ship){
    --ship;
  }else if(pos.getY(8, 0) > ship){
    ++ship;
  }
  blipbox.setLed(0, ship, brightness);
}

void keyTyped() {
  if(key == 'f')
    blipbox.fill(0xff);
  else if(key == 'c')
    blipbox.fill(0);
  else if(key == 43)
    ++width;
//    blipbox.brighten();
  else if(key == 45)
    --width;
//    blipbox.fade();
  else if(key > 47 && key < 58)
    brightness = (key - 48)*28+1; // keys 0-9 scaled to 1-253
}

void draw(){
  tick();
}

void tick(){
  blipbox.shiftRight(1);
  corner += round(random(-1, 1));
  corner = constrain(corner, 0, 7-width);
  blipbox.setLed(9, corner, brightness);
  blipbox.setLed(9, corner+width, brightness);
  blipbox.setLed(0, ship, brightness);
  delay(speed);
}

void stop() {
  blipbox.stop();
  super.stop();
}

