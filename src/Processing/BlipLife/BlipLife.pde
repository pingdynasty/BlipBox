ProcessingBlipBox blipbox;
Grid grid;
boolean stopped = true;
int lastx = -1;
int lasty = -1;
int brightness = 0xff;
int interval = 120;

void setup(){
  blipbox = new ProcessingBlipBox(this, 2);
  grid = new Grid(10, 8);
}

class Grid {
  int width, height;
  Cell[][] cells;
  Grid(int width, int height){
    this.width = width;
    this.height = height;
    cells = new Cell[width][height];
    for(int x=0; x<width; ++x)
      for(int y=0; y<height; ++y)
        cells[x][y] = new Cell(x, y);
    for(int x=0; x<width; ++x)
      for(int y=0; y<height; ++y)
        cells[x][y].setNeighbours(getNeighbours(x, y));    
  }
  
  Cell[] getNeighbours(int x, int y){
    List hood = new ArrayList();
    for(int xx=Math.max(x-1, 0); xx<Math.min(x+2, width); ++xx)
      for(int yy=Math.max(y-1, 0); yy<Math.min(y+2, height); ++yy)
        if(!(xx == x && yy == y))
          hood.add(cells[xx][yy]);
    Cell[] result = new Cell[hood.size()];
    hood.toArray(result);
    return result;
  }
  
  void draw(){
    for(int x=0; x<blipbox.getWidth(); ++x){
      byte bits = 0;
      for(int y=0; y<blipbox.getHeight(); ++y)
        if(cells[x][y].alive)
          bits |= 1<<y;
      blipbox.setLedColumn(x, bits);
    }
    /* alternative draw method using setLed()
  for(int x=0; x<blipbox.getWidth(); x++)
    for(int y=0; y<blipbox.getHeight(); y++)
      if(grid.cells[x][y].alive)
        blipbox.setLed(x, y, brightness);
      else
        blipbox.setLed(x, y, 0);
        */
  }
}

class Cell {
  int x, y;
  boolean alive;
  Cell[] hood;
  Cell(int x, int y){
    this.x = x;
    this.y = y;
  }
  void setNeighbours(Cell[] hood){
    this.hood = hood;
  }
  int neighbours(){
    int count = 0;
    for(int i=0; i<hood.length; ++i)
      if(hood[i].alive)
        count++;
    return count;
  }
  boolean survive(){
    int neighbours = neighbours();
    // B3/S23 Conway rules
    return (alive && neighbours == 2) || neighbours == 3;
    // B36/S23 HighLife rules
//    return (alive && neighbours == 2) || neighbours == 3 || neighbours == 6;
  }
}

//void touchClicked(Position pos){
void touchPressed(Position pos){
  println("pressed "+pos);
  int x = pos.getX(0, 10);
  int y = pos.getY(0, 8);
  if(x == 0 && y == 0){
    stopped = !stopped;
    println("stopped "+stopped);
    blipbox.sendCommand(Command.TOGGLE);
  }
}

void touchReleased(Position pos){
  println("released "+pos);
  lastx = -1;
  lasty = -1;
}

void touchDragged(Position origin, Position pos){
  println("dragged");
  int x = pos.getX(0, 10);
  if(x == 0){
    interval = pos.getY(800, 0);
  }else{
    int y = pos.getY(0, 8);
    if(lastx != x || lasty != y){
      if(grid.cells[x][y].alive){
        blipbox.setLed(x, y, stopped ? brightness : 0);
        grid.cells[x][y].alive = false;
      }else{
        blipbox.setLed(x, y, stopped ? 0 : brightness);
        grid.cells[x][y].alive = true;
      }
      lastx = x;
      lasty = y;
    }
  }
}

int notes[] = new int[]{ 60, 62, 64, 65, 67, 69, 71, 73 };

int getNote(int pos){
  return notes[pos];
}

void draw(){
  if(!stopped){
    Grid next = new Grid(grid.width, grid.height);
    for(int x=0; x<grid.width; x++)
      for(int y=0; y<grid.height; y++)
        next.cells[x][y].alive = grid.cells[x][y].survive();
    grid = next;
    blipbox.sendCommand(Command.START_LED_BLOCK);
    grid.draw();
    blipbox.sendCommand(Command.END_LED_BLOCK);
    delay(interval);
  }
}
