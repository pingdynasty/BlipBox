ProcessingBlipBox blipbox;
int x = 0;
int y = 0;
int value = 200;

void setup(){
  blipbox = new ProcessingBlipBox(this);
}

void draw(){
  blipbox.setLed(x, y, value);
  if(++x == 10){
    x = 0;
    if(++y == 8)
      y = 0;
  }
  if(blipbox.isScreenPressed())
    value = blipbox.getX(0, 200); // scale X pos
  delay(20);
}






