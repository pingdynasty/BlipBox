import controlP5.*;
import rwmidi.*;

rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int speed = 200;
int root = 60;
int channel = 0;
int velocity = 80;
int lastX, lastY;
boolean cycle = true;
int steplength = 8;

int[][] sequence = new int[12][8];

void setup() {
  size(800, 600);
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
//  blipbox = new ProcessingBlipBox(this, "/dev/tty.usbserial-A60081hj");
}

void keyTyped() {
  if(key == 'f')
    blipbox.fill(0xff);
  else if(key == '-')
    --velocity;
  else if(key == '+')
    ++velocity;
  else if(key == 'c')
    clear();
  else if(key == ' '){
    cycle = !cycle;
    println("cycle "+cycle);
  }
}

public void turnOn(int x, int y){
  sequence[x][y] = velocity;
  blipbox.setLed(x, y, velocity);
  println("on "+x+"/"+y);
}

void turnOff(int x, int y){
  sequence[x][y] = 0;
  blipbox.setLed(x, y, 0);
  println("off "+x+"/"+y);
}

void clear(){
  blipbox.clear();
  notesOff();
  for(int y=0; y<sequence[0].length; ++y)
    for(int x=0; x<sequence.length; ++x)
      sequence[x][y] = 0;
}

void reset(){
  lastX = -1;
  lastY = -1;
}

void release(Position pos){
  reset();
}

void position(Position pos){
  int x = pos.getX(0, 10);
  int y = 7 - pos.getY(0, 8);
  if(x != lastX || y != lastY){
    if(x > 7 || (sequence[x][y] == 0 && x > 1))
      turnOn(x, y);
    else
      turnOff(x, y);
    lastX = x;
    lastY = y;
  }
}

void draw() {
  background(0x0);
  fill(0xaa);
  line(120, 0, 120, height);
  translate(40, 60);

  blipbox.shiftRight(1);
  reset();
  
  for(int y=0; y<sequence[0].length; ++y){
    if(sequence[0][y] != 0)
      output.sendNoteOff(channel, y+root, 0);
    if(sequence[1][y] != 0)
      output.sendNoteOn(channel, y+root, sequence[1][y]);
    if(cycle && sequence[0][y] != 0){
      sequence[steplength+1][y] = sequence[0][y];
      blipbox.setLed(steplength-1, y, sequence[0][y]);
    }
  }
  
  for(int x=1; x<sequence.length; ++x){
    for(int y=0; y<sequence[0].length; ++y){
      ellipse(x*60, y*60, sequence[x][y]/2, sequence[x][y]/2);
      sequence[x-1][y] = sequence[x][y];
    }
  }

//  System.arraycopy(sequence, 1, sequence, 0, 10);
  delay(speed);
}

void notesOff(){
  for(int y=0; y<sequence[0].length; ++y)
    if(sequence[0][y] != 0)
      output.sendNoteOff(channel, y+root, 0);
}

void stop() {
  notesOff();
  blipbox.stop();
  super.stop();
}
