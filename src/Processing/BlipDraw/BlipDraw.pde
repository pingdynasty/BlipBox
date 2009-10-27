ProcessingBlipBox blipbox;

int lastx = -1;
int lasty = -1;
int brightness = 0xff;

void setup(){
  blipbox = new ProcessingBlipBox(this, 0);
//  blipbox.openSerialPort("/dev/tty.usbserial-A60081hd", 28800);
//  blipbox.openSerialPort("/dev/tty.usbserial-A60081hd", 38400);
//  blipbox.openSerialPort("/dev/tty.usbserial-A60081hd", 57600);
}

void draw(){
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, 10);
    int y = 7-blipbox.getY(0, 8);
    println("x/y "+x+"/"+y);
    if(lastx != x || lasty != y){
      if(blipbox.getLed(x, y) == 0)
        blipbox.setLed(x, y, brightness);
      else
        blipbox.setLed(x, y, 0);
      lastx = x;
      lasty = y;
    }
  }
  else{
    lastx = -1;
    lasty = -1;
  }
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

void stop() {
  blipbox.stop();
  super.stop();
}

