ProcessingBlipBox blipbox;
int speed = 120;

class Ball {
  float x, y;
  float speedX, speedY;
  public Ball(int x, int y){
    this.x = x;
    this.y = y;
    speedX = 0.2;
    speedY = 0.2;
  }
  public void draw(){
    x += speedX;
    y += speedY;    
    // check if we've hit a wall
    if(y < 0 || y > 7){
      speedY *= -1;
      y += speedY;
    }
    // check if we've hit a bat or goal
    switch(round(x)){
    case 1:
      leftBat.check(this);
      break;
    case 0:
      println("right side score!");
      x = 5;
      y = 4;
      break;
    case 8:
      rightBat.check(this);
      break;
    case 9:
      println("left side score!");
      x = 5;
      y = 4;
      break;
    }
    blipbox.setLed(round(x), round(y), 0xff);
  }
}

abstract class Bat {
  int x;
  float y;
  float speed;
  float width = 1.2;
  public Bat(int x, int y, float speed){
    this.x = x;
    this.y = (float)y;
    this.speed = speed;
  }
  public void check(Ball ball){
    if(abs(ball.y - this.y) <= this.width){
      ball.speedX *= -1;
      println("hit! "+ball.speedX+"/"+ball.);
    }
  }
  public void draw(){
    move();
    blipbox.setLed(x, round(y), 0xff);
  }
  public abstract void move();
}

class UserBat extends Bat {
  public UserBat(int x, int y){
    super(x, y, 1.0);
  }
  public void move(){
    int touch = 7 - blipbox.getY(0, 8);
    float distance = abs(touch - y);
    distance = min(distance, speed);
    if(touch > y)
      y += distance;
    else
      y -= distance;
  }
}

class PuterBat extends Bat {
  public PuterBat(int x, int y){
    super(x, y, 0.3);
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

Ball ball = new Ball(5, 4);
Bat leftBat = new UserBat(0, 4);
Bat rightBat = new PuterBat(9, 4);

void setup(){
  blipbox = new ProcessingBlipBox(this);
}

void draw(){
  blipbox.fade();
  leftBat.draw();
  rightBat.draw();
  ball.draw();
  delay(speed);
}









