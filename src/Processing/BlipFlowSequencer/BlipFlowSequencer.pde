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

int[][] sequence = new int[12][8];

void setup() {
  size(800, 600);
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 0);
//  blipbox = new ProcessingBlipBox(this, "/dev/tty.usbserial-A60081hj");
}

public void release(Position pos){
  lastX = -1;
  lastY = -1;
}

public void position(Position pos){
  int x = pos.getX(0, 10);
  int y = 7 - pos.getY(0, 8);
  if(x != lastX || y != lastY){
    if(sequence[x][y] == 0){
      sequence[x][y] = velocity;
      blipbox.setLed(x, y, velocity);
      println("on "+x+"/"+y);
    }else{
      sequence[x][y] = 0;
      blipbox.setLed(x, y, 0);
      println("off "+x+"/"+y);
    }
  lastX = x;
  lastY = y;
  }
}

void draw() {
  background(0x0);
  fill(0xaa);
  line(120, 0, 120, height);
  translate(40, 60);
  
  for(int y=0; y<sequence[0].length; ++y){
    if(sequence[0][y] != 0)
      output.sendNoteOff(channel, y+root, 0);
    if(sequence[1][y] != 0)
      output.sendNoteOn(channel, y+root, sequence[1][y]);
    if(cycle){
      sequence[11][y] = sequence[0][y];
      blipbox.setLed(9, y, sequence[10][y]);
    }
  }
  
  for(int x=1; x<sequence.length; ++x){
    for(int y=0; y<sequence[0].length; ++y){
      ellipse(x*60, y*60, sequence[x][y]/2, sequence[x][y]/2);
      sequence[x-1][y] = sequence[x][y];
    }
  }

//  System.arraycopy(sequence, 1, sequence, 0, 10);

  blipbox.shiftRight(1);
  delay(speed);
}

void stop() {
  for(int y=0; y<sequence[0].length; ++y)
    if(sequence[0][y] != 0)
      output.sendNoteOff(channel, y+root, 0);
  blipbox.stop();
  super.stop();
}
