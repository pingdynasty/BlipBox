package org.blipbox.processing;

import org.blipbox.*;
import processing.core.*;
import java.awt.*;

// class SimulatorFrame extends Frame {
//     public EmbeddedFrame(PApplet embedded){
// 	super("Embedded PApplet");
// 	setLayout(new BorderLayout());
// 	add(embedded, BorderLayout.CENTER);
// 	embedded.init();
//     }
// }

public class BlipSim extends PApplet {
    ProcessingBlipBox blipbox;
    int lastx = -1;
    int lasty = -1;
    int brightness = 0xff;
    int ratiox, ratioy;
    boolean draw = false;

    public BlipSim(ProcessingBlipBox blipbox){
	this.blipbox = blipbox;
    }

    public static Frame createSimulator(ProcessingBlipBox blipbox){
	PApplet embedded = new BlipSim(blipbox);
	Frame frame = new Frame("BlipBox Sim");
	frame.setLayout(new BorderLayout());
	frame.setResizable(true);
	frame.add(embedded, BorderLayout.CENTER);
	embedded.init();
	frame.show();
	return frame;
    }

    public void setup() {
	size(800, 640);
	ratiox = width/9;
	ratioy = height/7;
    }

    public void mousePressed(){
	mouseDragged();
    }

    private static final double MAX_SENSOR_VALUE = 1023.0;

    public void mouseDragged(){
//         BlipSensor sensor = blipbox.getBlipSensor(SensorType.X_SENSOR);
	double bx = mouseX * MAX_SENSOR_VALUE / width;
	double by = MAX_SENSOR_VALUE - mouseY * MAX_SENSOR_VALUE / height;
	blipbox.simulateTouch((int)bx, (int)by);
    }

    public void mouseReleased(){
	blipbox.simulateRelease();
    }


    public void draw() {
	background(0xff);
	fill(0x0);
	rect(0, 0, width, height);
	stroke(126);
	if(blipbox.isScreenPressed()){
	    line(blipbox.getX(0, width), 0, 
		 blipbox.getX(0, width), height);
	    line(0, blipbox.getY(height, 0), 
		 width-20, blipbox.getY(height, 0));
	}
	pushMatrix();
	translate(40, 40);
	for(int x=0; x<10; ++x){
	    for(int y=0; y<8; ++y){
		fill(204, 102, 0, 127);
		ellipse(x*ratiox, y*ratioy, 22, 22);
		fill(blipbox.getLed(x, 7-y), 0, 0);
		ellipse(x*ratiox, y*ratioy, 20, 20);
	    }
	}
	popMatrix();
    }

}