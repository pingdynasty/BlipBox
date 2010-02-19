ProcessingBlipBox blipbox;

void setup(){
//  blipbox = new ProcessingMonotar(this, 0);
  blipbox = new ProcessingTaquito(this, 0);
//  blipbox = new ProcessingBlipBox(this, 0);
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      println(sensor.getSensorName()+
        ":\t\t"+sensor.getValue()+
        "\t\t"+sensor.getRawValue());
    }
  }
  );
}

void draw(){
}


