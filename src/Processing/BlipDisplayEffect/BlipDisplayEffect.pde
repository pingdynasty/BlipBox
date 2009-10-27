ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this);  
}

void draw(){
}

void keyTyped() {
  println("typed " + int(key) + " " + keyCode);
  if(key > 47 && key < 58)
    blipbox.sendDisplayEffect(key - 47);
  else if(key == 'c')
    blipbox.clear();
}






