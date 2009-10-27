ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this);  
}

void draw(){
}

void keyTyped() {
  println("typed " + int(key) + " " + keyCode);
  if(key == 'f')
    blipbox.fill(0xff);
  else if(key == 'c')
    blipbox.fill(0);
  else if(key > 47 && key < 58)
    blipbox.fill((key - 48)*0x1c); // keys 0-9 scaled to 0-252
}





