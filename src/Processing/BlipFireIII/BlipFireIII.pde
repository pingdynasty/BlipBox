ProcessingBlipBox blipbox;

void setup(){
  blipbox = new ProcessingBlipBox(this, 0);
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      sleeptime = sensor.scale(0, 400);
    }
  }, org.blipbox.SensorType.Y_SENSOR);
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      intensity = sensor.scale(0, 256);
    }
  }, org.blipbox.SensorType.X_SENSOR);
}

int x;
int sleeptime = 100;
int intensity = 256;

void draw(){
  blipbox.shiftLeft(1);
  if(x++ % 2 == 0)
    blipbox.fade();
  for(int y=0; y<8; ++y)
    blipbox.setLed(0, y, (int)(noise(x, y)*intensity));
  delay(sleeptime);
}









