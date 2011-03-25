ProcessingBlipBox blipbox;
// variables to store coordinates
int lastx = -1;
int lasty = -1;
int brightness = 255;

void setup(){
  // initialise the library
  blipbox = new ProcessingBlipBox(this, 0);
}
 
void touchReleased(Position pos){
  // reset the coordinates
  // when the screen is no longer touched
  lastx = -1;
  lasty = -1;
}

void touchPressed(Position pos){
  println("pressed "+pos);
}
 
void touchDragged(Position pos){
  // get the screen coordinates
  // scaled to the size of the LED matrix
  int x = pos.getX(0, 10);
  int y = pos.getY(0, 8);
  if(lastx != x || lasty != y){
    // if the previous coordinates were different
    // we toggle the LED on or off
    if(blipbox.getLed(x, y) == 0)
      blipbox.setLed(x, y, brightness);
    else
      blipbox.setLed(x, y, 0);
   // save the current coordinates
    lastx = x;
    lasty = y;
  }
}

void keyTyped() {
  if(key == 'f'){
    blipbox.fill(0xff);
    println("fill");
  }else if(key == 'c'){
    blipbox.fill(0);
    println("clear");
  }else if(key == 43){
    blipbox.brighten();
    println("brighten");
  }else if(key == 45){
    blipbox.fade();
    println("fade");
  }else if(key > 47 && key < 58){
    brightness = (key - 48)*28+1; // keys 0-9 scaled to 1-253
    println("brightness "+brightness);
  }
}

void draw(){
}

void stop() {
  blipbox.stop();
  super.stop();
}

