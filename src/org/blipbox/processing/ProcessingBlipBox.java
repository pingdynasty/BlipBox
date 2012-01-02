package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

import processing.core.*;
import java.awt.*;

public class ProcessingBlipBox extends BlipBox {

    private PApplet parent;

    public ProcessingBlipBox(SensorConfiguration config, PApplet parent) {
        super(config);
        this.parent = parent;
        if(parent != null)
            parent.registerDispose(this);
        addInputHandler(new DynamicBlipAction(parent));
    }

    public ProcessingBlipBox(PApplet parent) {
        this(new BlipBoxSensorConfiguration(), parent);
    }

    public ProcessingBlipBox(PApplet parent, String serialport) {
        this(parent);
        try{
            openSerialPort(serialport);
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public ProcessingBlipBox(PApplet parent, int portindex) {
        this(parent);
        try{
            openSerialPort(getSerialPorts().get(portindex));
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public void stop(){
        closeSerialPort();
    }

    public void dispose() {
        closeSerialPort();
    }

    public Frame createSimulator(){
	PApplet embedded = new BlipSim(this);
	Frame frame = new Frame("BlipBox Sim");
	frame.setLayout(new BorderLayout());
	frame.setResizable(true);
	frame.add(embedded, BorderLayout.CENTER);
	frame.setSize(600, 400);
	embedded.init();
	frame.show();
	return frame;
    }

}