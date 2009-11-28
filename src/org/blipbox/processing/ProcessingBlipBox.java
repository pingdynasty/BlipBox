package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

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
}