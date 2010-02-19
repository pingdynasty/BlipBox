import oscP5.*;
import netP5.*;
import supercollider.*;

Bus xbus, ybus;
float xval, yval;
ProcessingBlipBox blipbox;

void setup() {
  size(600, 400);
  Server server = Server.local;
  xbus = new Bus("control", server, 1);
  ybus = new Bus("control", server, 1);
  xbus.index = 0;
  ybus.index = 1;
  blipbox = new ProcessingBlipBox(this, 0);
}

public void release(Position pos){
  println("release "+pos);
//   xbus.set(0.0);
//   ybus.set(0.0);
}

public void position(Position pos){
  println(pos.toString());
  xval = pos.getX();
  yval = pos.getY();
  update();
  blipbox.setLed(pos.getX(0, 10), 7-pos.getY(0, 8), 255);
}

public void update(){
    xbus.set(xval);
    ybus.set(yval);
}

public void mouseDragged(){
    mousePressed();
}

public void mousePressed(){
  xval = mouseX/(float)width;
  yval = (height-mouseY)/(float)height;
  update();
}

void draw(){
  background(0);
  stroke(255);
  int x = (int)(xval * width);
  int y = height - (int)(yval * height);
  line(x, 0, x, height);
  line(0, y, width, y);
  blipbox.fade();
}

void stop (){
  ybus.free();
  xbus.free();
}




