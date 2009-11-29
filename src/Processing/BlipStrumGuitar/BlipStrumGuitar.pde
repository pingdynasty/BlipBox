import oscP5.*;
import netP5.*;
import supercollider.*;

Synth guitarX;
Synth guitarY;
ProcessingBlipBox blipbox;

/* SuperCollider // arg1(0.1 - 10), arg2(1 - 13)
*/

void setup() {
  size(800, 200);
  guitarX = new Synth("strumguitar");
  guitarY = new Synth("strumguitar");
  guitarX.set("root", 49);
  guitarY.set("root", 52);
  guitarX.create();
  guitarY.create();
  blipbox = new ProcessingBlipBox(this, 0);
}

public void position(Position pos){
  guitarX.set("pos", pos.getX());
  guitarY.set("pos", pos.getY());
  blipbox.setLed(pos.getX(0, 10), pos.getY(0, 9), 255);
}

public void mouseMoved(){
  guitarX.set("pos", mouseX/(float)width);  
  guitarY.set("pos", mouseY/(float)height);  
}

int counter;

void draw (){
  if(counter++ % 127 == 0)
  blipbox.fade();
  background(0);
  stroke(255);
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, width);
    int y = height - blipbox.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}

void stop (){
  guitarX.free();
  guitarY.free();
}



