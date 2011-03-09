ProcessingBlipBox blipbox;

int lastx = -1;
int lasty = -1;
int brightness = 0xff;
int ratiox, ratioy;
 
void setup(){
  size(800, 640);
  blipbox = new ProcessingBlipBox(this, 0);
  ratiox = (width-40)/9;
  ratioy = (height-40)/7;
}

void release(Position pos){
  println("released");
}

void position(Position pos){
  println(pos.toString());
}

void keyTyped() {
  if(key == 'f')
    blipbox.fill(0xff);
  else if(key == 'c')
    blipbox.fill(0);
  else if(key == 43)
    blipbox.brighten();
  else if(key == 45)
    blipbox.fade();
  else if(key > 47 && key < 58)
    brightness = (key - 48)*28+1; // keys 0-9 scaled to 1-253
}

boolean painting;
void mouseReleased(){
  painting = false;
}

void mousePressed(){
  painting = blipbox.getLed(getScreenX(), getScreenY()) == 0;
  mouseDragged();
}

int getScreenX(){
  return mouseX / ratiox;
}

int getScreenY(){
  return (height-mouseY) / ratioy;
}

void mouseDragged(){
  int bx = getScreenX();
  int by = getScreenY();
  if(painting)
    blipbox.setLed(bx, by, brightness);
  else
    blipbox.setLed(bx, by, 0);
}

void draw(){
  background(0xff);
  translate(20, -20);
  for(int x=0; x<10; ++x){
    for(int y=0; y<8; ++y){
      fill(blipbox.getLed(x, y));
      ellipse(x*ratiox, height-y*ratioy, 20, 20);
    }
  }
}

void stop() {
  blipbox.stop();
  super.stop();
}

