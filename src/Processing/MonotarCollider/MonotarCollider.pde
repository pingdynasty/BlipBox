import oscP5.*;
import netP5.*;
import supercollider.*;

ProcessingBlipBox blipbox;
BlipSensor potsensor;
Bus[] busses = new Bus[4];

int X =   0;
int Y =   1;
int Z =   2;
int POT = 3;

// Supercollider:
// SynthDef(\strumguitar, {
//      arg out=0, pos=0, root=52, sustain=0.05, gain=0.5;
// 	var pitch, data;
// 	pitch = [ root, root+5, root+10, root+15, root+19, root+24 ];		// e a d g b e
// 	data = Mix.fill(pitch.size, { arg i;
// 		var trigger, pluck, period, string;
// 		// place trigger points from 0.25 to 0.75
// 		trigger = HPZ1.kr(pos > (0.25 + (i * 0.1))).abs;
// 		pluck = PinkNoise.ar(Decay.kr(trigger, sustain/2, gain));
// 		period = pitch.at(i).midicps.reciprocal;
// 		string = CombL.ar(pluck, period, period, 4, gain);
// 		Pan2.ar(string, i * 0.2 - 0.5);
// 	});
// 	data = LPF.ar(data, 12000);
// 	data = LeakDC.ar(data);
// 	Out.ar(out, data);
// }).store;
// e = Synth(\strumguitar, [\out, 0, \root, 47]);
// e.map(\pos, Bus.new(\control, 1, 1));
// e.map(\root, Bus.new(\control, 3, 1));
// e.map(\sustain, Bus.new(\control, 0, 1));

void setup(){
  blipbox = new ProcessingMonotar(this, 0);
  size(800, 200);
  Server server = Server.local;

  potsensor = blipbox.getBlipSensor("analog1");
  for(int i=0; i<busses.length; ++i){
    busses[i] = new Bus("control", server, 1);
    busses[i].index = i;
  }

  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      println(sensor.getSensorName()+
        ":\t\t"+sensor.getValue()+
        "\t\t"+sensor.getRawValue());
      busses[Z].set(sensor.getValue());
    }
  }
  , "z");
  blipbox.addInputHandler(new BlipBoxInput(){
    public void sensorChange(BlipSensor sensor){
      println(sensor.getSensorName()+
        ":\t\t"+sensor.getValue()+
        "\t\t"+sensor.getRawValue());
      busses[POT].set(127-sensor.scale(65, 90));
    }
  }
  , "analog1");
}

public void release(Position pos){
  println("release "+pos);
  busses[Z].set(0.0);
}

public void position(Position pos){
  println(pos.toString());
  busses[X].set(pos.getX());
  busses[Y].set(pos.getY());
}

public void mouseDragged(){
  mousePressed();
}

public void mousePressed(){
  busses[X].set(mouseX/(float)width);  
  busses[Y].set((height-mouseY)/(float)height);  
  busses[Z].set(1.0);
}

public void mouseReleased(){
  busses[Z].set(0.0);
}

void draw(){
  background(0);
  stroke(255);
  int lines = 6;
  for(int i=1; i<lines; ++i){
    line(i*width/lines, 0, i*width/lines, height);
  }
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, width);
    int y = height - blipbox.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}

void stop(){
  for(int i=0; i<busses.length; ++i)
    busses[i].free();
  blipbox.stop();
}


