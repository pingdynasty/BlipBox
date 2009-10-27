ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this);  
}

void draw(){
}

int pos = 0;

void keyTyped() {
  println("typed " + int(key) + " " + keyCode);
  if(key > 47 && key < 58)
    pos = key - 48; // keys 0-9
  else
    blipbox.writeCharacter(pos, key);
}






