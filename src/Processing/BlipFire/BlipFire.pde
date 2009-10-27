ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this, 0);
}

int x;
int sleeptime = 100;
int intensity = 256;

void position(int x, int y){
  sleeptime = blipbox.getY(0, 400);
  intensity = blipbox.getX(0, 256);
}

void draw(){
  blipbox.shiftLeft(1);
  if(x++ % 2 == 0)
    blipbox.fade();
  for(int y=0; y<8; ++y)
    blipbox.setLed(0, y, (int)(noise(x, y)*intensity));
  delay(sleeptime);
}







