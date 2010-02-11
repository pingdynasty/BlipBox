import rwmidi.*;

ProcessingBlipBox blipbox;
MidiOutput output;
BlipSensor potsensor;

int channel  = 0;
int xCC      = 1;
int potCC    = 33;
int yCC      = 34;
int zCC      = 35;
int range    = 6;
int root     = 40;
int tone     = -1;
int velocity = 100;
int girth    = 40;
boolean legato = false;

class TarString {
  int pos;
  int root;
  int tone = -1;
  TarString(int pos, int root){
    this.pos = pos;
    this.root = root;
  }
  void play(int tone){
    tone += root;
    if(tone == this.tone)
      return; // already playing this tone
    if(legato){
      noteOn(tone);
      if(this.tone != -1)
        noteOff(this.tone);
    }else{
      if(this.tone != -1)
        noteOff(this.tone);
      noteOn(tone);
    }
    this.tone = tone;
    println("play "+pos+"/"+tone);
  }
  void release(){
    println("release "+this.tone);
    if(tone != -1)
        noteOff(tone);
    tone = -1;
  }
}

TarString[] strings;
TarString playing;

void setup(){
  blipbox = new ProcessingMonotar(this, "/dev/tty.usbserial-A6004bII");
  size(800, 400);
  output = RWMidi.getOutputDevices()[0].createOutput();
  
  strings = new TarString[2];
  int spacing = (height-2*girth)/strings.length;
  for(int i=0; i<strings.length; ++i)
    strings[i] = new TarString(i*spacing+2*girth, i*5);

  potsensor = blipbox.getBlipSensor("analog1");
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
//      println(sensor.getSensorName()+
//        ":\t\t"+sensor.getValue()+
//        "\t\t"+sensor.getRawValue());

//        output.sendController(channel, zCC, sensor.scale(0, 127));
    }
  }
  , "z");
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      if(sensor.getValue() == 0)
        release();
      else
        play(sensor.scale(root, root+range));
    }
  }
  , "analog1");
}

void noteOn(int tone){
  if(this.tone != -1 && !legato)
    noteOff(this.tone);
  output.sendNoteOn(channel, tone, velocity);
  this.tone = tone;
}

void noteOff(int tone){
  output.sendNoteOff(channel, tone, 0);
}

void play(TarString string, int tone){
  if(playing != null)
      playing.release();
  playing = string;
  playing.play(tone);
}

void play(int tone){
  if(playing != null)
    playing.play(tone);
}

void release(){
  if(playing != null)
      playing.release();
  playing = null;
  output.sendController(channel, yCC, 0);
}

void position(Position pos){
  println(pos.toString());
  int y = pos.getY(height, 0);
  for(int i=0; i<strings.length; ++i){
    if(abs(y-strings[i].pos) < girth)
      play(strings[i], potsensor.scale(root, range));
  }
  output.sendController(channel, xCC, pos.getX(0, 127));
}

void release(Position pos){
  println("release "+pos);
  release();
}

void keyTyped(){
  if(key == ' '){
    legato = !legato;
    println("legato "+legato);
  }else if(key == 'x' || key == 'X'){
    output.sendController(channel, 123, 0);
    println("all notes off");
  }
}

/* 
public void mouseDragged(){
  mousePressed();
}
public void mousePressed(){
  busses[X].set(mouseX/(float)width);  
  busses[Y].set((height-mouseY)/(float)height);  
  busses[Z].set(1.0);
}
public void mouseReleased(){
  release();
}
*/
 
void draw(){
  background(0);
  stroke(#FFCC00);
  strokeWeight(girth);
  for(int i=0; i<strings.length; ++i)
    line(0, strings[i].pos, width, strings[i].pos);
  stroke(255);
  strokeWeight(1);
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, width);
    int y = height - blipbox.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
  stroke(127);
  line(potsensor.scale(0, width), 0, potsensor.scale(0, width), height);
}

void stop(){
  blipbox.stop();
}


