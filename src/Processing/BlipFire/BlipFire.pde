ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this, 2);
}

int x;
int sleeptime = 200;
int intensity = 256;

void touch(Position pos){
  sleeptime = pos.getY(0, 400);
  intensity = pos.getX(256, 0);
  println("x/y "+pos);
}

void draw(){
  blipbox.sendCommand(Command.START_LED_BLOCK);
  blipbox.shiftLeft(1);
  blipbox.fade(4);
  x++;
  for(int y=0; y<8; ++y)
    blipbox.setLed(9, y, (int)(noise(x, y)*intensity));
  blipbox.sendCommand(Command.END_LED_BLOCK);
  delay(sleeptime);
}







