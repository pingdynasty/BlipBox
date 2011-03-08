rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int brightness = 32;
int speed = 200;
int root = 60;
int channel = 0;
int velocity = 80;

int ship = 3;
int corner = 0;
int width = 7;

void setup(){
  size(800, 600);
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
}

void release(Position pos){
  println("released");
}

void _position(Position pos){
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
  else if(key == '+')
    ++width;
//    blipbox.brighten();
  else if(key == '-')
    --width;
//    blipbox.fade();
  else if(key > 47 && key < 58)
    brightness = (key - 48)*28+1; // keys 0-9 scaled to 1-253
}

void crashAndBurn(){
  println("crash!");
  blipbox.fill(0);
//  ++width;
}

void draw(){
  background(0x0);
  fill(0xaa);
  line(120, 0, 120, height);
  translate(40, 60);

  for(int x=0; x<10; ++x){
    for(int y=0; y<8; ++y){
      ellipse(x*60, y*60, blipbox.getLed(x, y)/2, blipbox.getLed(x, y)/2);
    }
  }
  tick();
  delay(speed);
}

void tick(){
  blipbox.shiftRight(1);
  // move ship
  if(blipbox.getY(8, 0) < ship)
    --ship;
  else if(blipbox.getY(8, 0) > ship)
    ++ship;
  ship = constrain(ship, 0, 8);
  // check for walls
  if(blipbox.getLed(0, ship) == brightness)
    crashAndBurn();
  // draw ship
  blipbox.setLed(0, ship, 0xff);
  // draw walls
  corner += round(random(-1, 1));
  corner = constrain(corner, 0, 7-width);
  blipbox.setLed(9, corner, brightness);
  blipbox.setLed(9, corner+width, brightness);
}

void stop() {
  blipbox.stop();
  super.stop();
}

