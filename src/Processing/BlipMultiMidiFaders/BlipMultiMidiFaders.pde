import rwmidi.*;

rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int speed = 100;
int channel = 0;

Fader[] cctable = new Fader[127];
Page[] pages = new Page[4];
int page = 0; // current page index;

void setup() {
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  println("output: "+RWMidi.getOutputDevices()[0]);
  blipbox = new ProcessingBlipBox(this, 0);
//  blipbox = new ProcessingBlipBox(this, "/dev/tty.usbserial-A60081h8");
  pages[0] = new Page(1);
  pages[1] = new Page(33);
  pages[2] = new Page(41);
  pages[3] = new Page(49);
//  for(int i=0; i<pages.length; ++i)
//    pages[i] = new Page(i*8+1);
  setPage(0);
}

class Page {
  Fader[] faders = new Fader[8];
  public Page(int cc){
    // create a page with faders
    cc += faders.length-1;
    for(int i=0; i<faders.length; ++i)
      setFader(cc-i, i);
  }

  public void setVisibility(boolean visible){
    for(int i=0; i<faders.length; ++i)
      faders[i].visible = visible;
  }

  public void show(){
    setVisibility(true);
    draw();
  }

  public void hide(){
    setVisibility(false);
  }

  public void setFader(int cc, int position){
    faders[position] = new Fader(cc, position);
    cctable[cc] = faders[position];
  }

  public void updateFaders(Position pos){
    if(pos.getX(0, 30) % 3 != 1)
      return; // ignore everything in between virtual fader tracks.
    int col = pos.getX(0, 10);
    Fader fader = faders[col];
    fader.update(pos.getY(127, 0)-1);
    output.sendController(channel, fader.cc, fader.value);
  }

  public void draw(){
    for(int i=0; i<faders.length; ++i)
      faders[i].draw();
  }
}

public void drawPage(){
  pages[page].draw();
}

class Fader {
  int cc;
  int position;
  int value;
  boolean visible;

  public Fader(int cc, int position){
    this.cc = cc;
    this.position = position;
  }

  public void update(int value){
    if(value >= 0 && value < 128 && this.value != value){
      this.value = value;
      if(visible)
        draw();
//      println("updated "+this);
    }
  }

  final int step = 128 / 8;
  public void draw(){
    int i = 0;
    int bits = 0;
    while(i < value/step && i < 8)
      bits |= 1<<i++;
    blipbox.setLedColumn(9-position, bits);
    blipbox.setLed(position, i, ((value % step)*256)/step);
  }

  public String toString(){
    return position+"/"+cc+":"+value;
  }
}

public Fader getFaderForCC(int cc){
  return cctable[cc];
}

public void controllerChangeReceived(Controller cc){
//  println("received "+cc);
  Fader fader = getFaderForCC(cc.getCC());
  if(fader != null)
    fader.update(cc.getValue());
}

public void tap(Position pos){
  if(pos.getX(0, 10) >= 8)
    updatePages(pos);
}

public void position(Position pos){
  if(pos.getX(0, 10) < 8)
    pages[page].updateFaders(pos);
}

public void updatePages(Position pos){
  int index = pos.getY(0, pages.length);
  if(index != page)
    setPage(index);
}

public void setPage(int index){
//  println("set page "+index);
  pages[page].hide();
  page = index;
  pages[page].show();
  int bits = 0;
  for(int i=0; i<9; ++i){
    if(7-i == page*2 || 6-i == page*2)
      bits |= 1 << i;
  }
  blipbox.setLedColumn(0, bits);
  blipbox.setLedColumn(1, bits);
}

void draw() {
  //  blipbox.fade();
  //  drawFaders();
  delay(speed);
}














