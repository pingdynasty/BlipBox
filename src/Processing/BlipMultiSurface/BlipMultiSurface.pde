import controlP5.*;
import rwmidi.*;

rwmidi.MidiInput input;
rwmidi.MidiOutput output;
ProcessingBlipBox blipbox;

int speed = 100;
int channel = 0;

Control[] cctable = new Control[127];
Surface[] pages = new Surface[4];
int page = 0; // current page index;

ControlP5 controlP5;
Slider[] sliders;

void setup() {
  size(720, 200);
  controlP5 = new ControlP5(this);
  input = RWMidi.getInputDevices()[0].createInput(this);
  output = RWMidi.getOutputDevices()[0].createOutput();
  blipbox = new ProcessingBlipBox(this, 
  "/dev/tty.usbserial-A60081h8");
//  blipbox = new ProcessingBlipBox(this, "/dev/tty.usbserial-A60081hj");
  pages[0] = new Surface(33, 34);
  pages[1] = new Surface(35, 36);
  pages[2] = new Surface(37, 38);
  pages[3] = new Surface(39, 40);
  setPage(0);
}

class Control {
  int cc, value;
  Slider slider;
  public Control(int cc){
    this.cc = cc;
    slider = controlP5.addSlider("cc"+cc, 0, 127, 0, 80*(cc-32), 50, 10, 100);
    slider.setId(cc);
  }
}

class Surface {
  Control x, y;

  public Surface(int cc1, int cc2){
    x = new Control(cc1);
    cctable[cc1] = x;
    y = new Control(cc2);
    cctable[cc2] = y;
  }

  public void update(Position pos){
//    x.value = pos.getX(0, 128);
    x.value = (int)(pos.getX(0.0, 1.1)*127.0);
    y.value = pos.getY(0, 128);
    output.sendController(channel, x.cc, x.value);
    output.sendController(channel, y.cc, y.value);
    x.slider.changeValue(x.value);
    y.slider.changeValue(y.value);
  }

  public void draw(){
    blipbox.fade();
    blipbox.setLed(x.value * 8 / 128, 7 - y.value * 8 / 128, 0xff);
  }
}

public Control getControlForCC(int cc){
  return cctable[cc];
}

public void controllerChangeReceived(rwmidi.Controller cc){
//  println("received "+cc);
  Control control = getControlForCC(cc.getCC());
  if(control != null){
    control.value = cc.getValue();
//    setBroadcast(false);
    control.slider.changeValue(control.value);
  }
}

public void tap(Position pos){
  if(pos.getX(0, 10) > 8)
    updatePages(pos);
}

public void position(Position pos){
  if(pos.getX(0, 10) < 9)
    pages[page].update(pos);
}

public void updatePages(Position pos){
  int index = pos.getY(0, pages.length);
  if(index != page)
    setPage(index);
}

int bits = 0;

public void setPage(int index){
//  println("set page "+index);
  page = index;
  bits = 0;
  for(int i=0; i<9; ++i){
    if(7-i == page*2 || 6-i == page*2)
      bits |= 1 << i;
  }
  pages[page].draw();
}

void controlEvent(ControlEvent event) {
  println("got a control event from controller with id "+event.controller().id());
  Control control = getControlForCC(event.controller().id());
  if(control != null){
    control.value = (int)event.controller().value();
    output.sendController(channel, control.cc, control.value);
    println("set cc "+control.cc+" to "+control.value);
  }
}


void draw() {
  background(0xaaddee);
  blipbox.setLedColumn(0, bits);
  blipbox.setLedColumn(1, bits);
  pages[page].draw();
  delay(speed);
}






