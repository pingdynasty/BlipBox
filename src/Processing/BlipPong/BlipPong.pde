ProcessingBlipBox blipbox;

Ball ball = new Ball(5, 4);
Bat leftBat = new UserBat(0, 4);
Bat rightBat = new PuterBat(9, 4);

int speed = 50; // millisecond pause: shorter = faster

void setup(){
  blipbox = new ProcessingBlipBox(this,0);  
}

class Ball {
  float x, y;
  float speedX, speedY;
  public Ball(int x, int y){
    this.x = x;
    this.y = y;
    speedX = 0.15;
    speedY = 0.25;
  }
  public void draw(){
    x += speedX;
    y += speedY;
    if(y <= 0){
      y = 0;
      speedY *= -1;
    }
    else if(y >= 7){
      y = 7;
      speedY *= -1;
    }
    if(x <= 1){
      if(abs(y - leftBat.y) <= leftBat.width){
        speedX *= -1;
        println("hit!");
      }
      else if(x <= 0){
        println("miss!");
        x = 5;
        y = 4;
      }
    }
    else if(x >= 8){
      if(abs(y - rightBat.y) <= rightBat.width){
        speedX *= -1;
        println("hit!");
      }
      else if(x >= 9){
        println("miss!");
        x = 5;
        y = 4;
      }      
    }
    blipbox.setLed(round(x), round(y), 0xff);
  }
}

abstract class Bat {
  int x;
  float y;
  float width = 1.2;
  public Bat(int x, int y){
    this.x = x;
    this.y = (float)y;
  }
  public void draw(){
    move();
    blipbox.setLed(x, round(y), 0xff);
  }
  public abstract void move();
}

class UserBat extends Bat {
  public UserBat(int x, int y){
    super(x, y);
  }
  public void move(){
    int touch = 7 - blipbox.getY(0, 8);
    if(y > touch)
      --y;
    else if(y < touch)
      ++y;
  }
}

class PuterBat extends Bat {
  float speed = 0.3;
  public PuterBat(int x, int y){
    super(x, y);
  }
  public void move(){
    float distance = abs(ball.y - y);
    distance = min(distance, speed);
    if(ball.y > y)
    y += distance;
    else
    y -= distance;
  }
}

void draw(){
  blipbox.fade();
  leftBat.draw();
  rightBat.draw();
  ball.draw();
  delay(speed);
}



