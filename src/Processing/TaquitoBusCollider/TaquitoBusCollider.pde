import oscP5.*;
import netP5.*;
import supercollider.*;

Bus freq;
Bus amp;
ProcessingTaquito taquito;

void setup() {
  size(800, 200);
  Server server = Server.local;
  // for SuperColliderAU:
  // Server server = new Server("127.0.0.1", 9989);
  freq = new Bus("control", server, 1);
  amp = new Bus("control", server, 1);
  freq.index = 0;
  amp.index = 1;
  amp.set(0.0);
  taquito = new ProcessingTaquito(this, "/dev/tty.usbserial-FTDY52CQ");
}

public void release(Position pos){
  println("release "+pos);
  amp.set(0.0);
}

public void position(Position pos){
  println(pos.toString());
  if(pos.getX() != 0.0)
    freq.set(pos.getX());
  amp.set(pos.getY());
}

public void mouseDragged(){
  mousePressed();
}

public void mousePressed(){
  freq.set(mouseX/(float)width);  
  amp.set((height-mouseY)/(float)height);  
}

public void mouseReleased(){
  amp.set(0.0);
}

void draw(){
  background(0);
  int lines = 6;
  for(int i=1; i<lines; ++i){
    line(i*width/lines, 0, i*width/lines, height);
  }
  stroke(255);
  if(taquito.isScreenPressed()){
    int x = taquito.getX(0, width);
    int y = height - taquito.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}

void stop (){
  amp.set(0.0);
  amp.free();
  freq.free();
}





