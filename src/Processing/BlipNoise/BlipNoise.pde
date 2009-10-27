ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this, 0);
}

int x;

void draw(){
  int sleeptime = blipbox.getX(0, 400);
  blipbox.shiftLeft(1);
  for(int y=0; y<8; ++y)
    blipbox.setLed(0, y, (int)(noise(x, y)*256));
  x++;
  delay(sleeptime);
}


