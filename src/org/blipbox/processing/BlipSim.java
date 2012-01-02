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
    boolean draw = false;

    public BlipSim(ProcessingBlipBox blipbox){
	this.blipbox = blipbox;
    }

    public void setup() {
	size(600, 400);
    }

    public void mousePressed(){
	mouseDragged();
    }

    private static final double MAX_SENSOR_VALUE = 1023.0;
    public void mouseDragged(){
// 	double bx =  (mouseX - width*0.1) * MAX_SENSOR_VALUE / (width*0.8);
// 	double by = MAX_SENSOR_VALUE - (mouseY - height*0.1) * MAX_SENSOR_VALUE / (height*0.8);
	double bx =  mouseX * MAX_SENSOR_VALUE / width;
	double by = MAX_SENSOR_VALUE - mouseY * MAX_SENSOR_VALUE / height;
	bx = Math.min(MAX_SENSOR_VALUE, Math.max(0, bx));
	by = Math.min(MAX_SENSOR_VALUE, Math.max(0, by));
	blipbox.simulateTouch((int)bx, (int)by);
    }

    public void mouseReleased(){
	blipbox.simulateRelease();
    }

    public void draw(){
	background(0x00);
	fill(0x0);
	stroke(126);
	double ratiox = (width*0.88)/9;
	double ratioy = (height*0.88)/7;
	int radius = (int)(Math.min(ratiox, ratioy)*0.8);
	if(blipbox.isScreenPressed()){
	    line(blipbox.getX(0, width), 0, 
		 blipbox.getX(0, width), height);
	    line(0, blipbox.getY(height, 0), 
		 width-20, blipbox.getY(height, 0));
	}
	pushMatrix();
	translate((int)ratiox/2, (int)ratioy/2);
	for(int x=0; x<10; ++x){
	    for(int y=0; y<8; ++y){
		fill(204, 102, 0, 127);
		ellipse((int)(x*ratiox), (int)(y*ratioy), radius+2, radius+2);
		fill(blipbox.getLed(x, 7-y), 0, 0);
		ellipse((int)(x*ratiox), (int)(y*ratioy), radius, radius);
	    }
	}
	popMatrix();
    }

}