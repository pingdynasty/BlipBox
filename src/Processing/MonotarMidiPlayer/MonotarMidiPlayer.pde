import rwmidi.*;

ProcessingBlipBox blipbox;
MidiOutput output;
BlipSensor potsensor;

int channel  = 0;
int xCC      = 1;
int potCC    = 33;
int yCC      = 34;
int zCC      = 35;
int range    = 24;
int root     = 36;
int velocity = 63;
int girth    = 58;
int stringcount = 4;
boolean legato = false;

class TarString {
  int pos;
  int root;
  int tone = -1;
  TarString(int pos, int root){
    this.pos = pos;
    this.root = root;
  }
  boolean playing(){
    return tone != -1;
  }
  void strum(int tone){
    if(playing())
      play(tone);
  }
  void play(int tone){
    tone += root;
    if(tone == this.tone)
      return; // already playing this tone
    if(legato){
      noteOn(tone);
      if(playing())
        noteOff(this.tone);
    }else{
      if(playing())
        noteOff(this.tone);
      noteOn(tone);
    }
    this.tone = tone;
    println("play "+pos+"/"+root+": "+tone);
  }
  void release(){
    if(playing()){
      //    println("release "+tone);
        noteOff(tone);
        tone = -1;
    }
  }
}

TarString[] strings;

void setup(){
  blipbox = new ProcessingMonotar(this, "/dev/tty.usbserial-A6004bII");
  size(800, 400);
  output = RWMidi.getOutputDevices()[0].createOutput();
  
  strings = new TarString[stringcount];
  int margin = 60;
  int spacing = (height-2*margin)/(strings.length-1);
  for(int i=0; i<strings.length; ++i)
    strings[i] = new TarString(i*spacing+margin, (strings.length-1-i)*5);

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
      if(sensor.getValue() != 0 && legato)
        for(int i=0; i<strings.length; ++i)
          strings[i].strum(sensor.scale(root+range, root));
    }
  }
  , "analog1");
}

void noteOn(int tone){
  output.sendNoteOn(channel, tone, velocity);
}

void noteOff(int tone){
  output.sendNoteOff(channel, tone, 0);
}

void release(){
  for(int i=0; i<strings.length; ++i)
    strings[i].release();
  // output.sendController(channel, yCC, 0);
}

void position(Position pos){
//  println(pos.toString());
  int y = pos.getY(height, 0);
  for(int i=0; i<strings.length; ++i){
    if(abs(y-strings[i].pos) < girth/2)
      strings[i].play(potsensor.scale(root+range, root));
//      strings[i].play(potsensor.scale(root, root+range));
    else
      strings[i].release();
  }
  output.sendController(channel, xCC, pos.getX(0, 127));
}

void release(Position pos){
//  println("release "+pos);
  release();
}

void keyTyped(){
  if(key == ' '){
    legato = !legato;
    println("legato "+legato);
  }else if(key == 'x' || key == 'X'){
    release();
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
  color on = #FFFF00;
  color off = legato ? #CCFF00 : #FFCC00;
  strokeWeight(girth);
  for(int i=0; i<strings.length; ++i){
    stroke(strings[i].playing() ? on : off);
    line(0, strings[i].pos, width, strings[i].pos);
  }
  stroke(255);
  strokeWeight(1);
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, width);
    int y = height - blipbox.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
  stroke(127);
  strokeWeight(2);
  line(potsensor.scale(width, 0), 0, potsensor.scale(width, 0), height);
}

