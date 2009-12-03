import oscP5.*;
import netP5.*;
import supercollider.*;

Synth guitarX;
Synth guitarY;
ProcessingBlipBox blipbox;

/* SuperCollider // pos (0.0-1.0)
SynthDef(\strumguitar, {
     arg pos=0, root=52;
	var pitch, out;
	pitch = [ root, root+5, root+10, root+15, root+19, root+24 ];		// e a d g b e
	out = Mix.fill(pitch.size, { arg i;
		var trigger, pluck, period, string;
		// place trigger points from 0.25 to 0.75
		trigger = HPZ1.kr(pos > (0.25 + (i * 0.1))).abs;
		pluck = PinkNoise.ar(Decay.kr(trigger, 0.05));
		period = pitch.at(i).midicps.reciprocal;
		string = CombL.ar(pluck, period, period, 4);
		Pan2.ar(string, i * 0.2 - 0.5);
	});
	LPF.ar(out, 12000);
	LeakDC.ar(out);
	Out.ar(0, out ! 2);
}).store;
*/

void setup() {
  size(800, 200);
  guitarX = new Synth("strumguitar");
  guitarY = new Synth("strumguitar");
  guitarX.set("root", 49);
  guitarY.set("root", 52);
  guitarX.create();
  guitarY.create();
  blipbox = new ProcessingBlipBox(this, 0);
}

public void position(Position pos){
  guitarX.set("pos", pos.getX());
  guitarY.set("pos", pos.getY());
//  blipbox.setLed(pos.getX(0, 10), 8-pos.getY(0, 9), 255);
}

public void mouseMoved(){
  guitarX.set("pos", mouseX/(float)width);  
  guitarY.set("pos", mouseY/(float)height);  
}

int counter;

void draw (){
  blipbox.fade();
  blipbox.setLed(blipbox.getX(0, 10), 8-blipbox.getY(0, 9), 255);

  background(0);
  stroke(255);
  if(blipbox.isScreenPressed()){
    int x = blipbox.getX(0, width);
    int y = height - blipbox.getY(0, height);
    line(x, 0, x, height);
    line(0, y, width, y);
  }
}

void stop (){
  guitarX.free();
  guitarY.free();
}



