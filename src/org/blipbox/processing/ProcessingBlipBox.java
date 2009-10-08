package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

public class ProcessingBlipBox extends BlipBox {

    private PApplet parent;
    private int DEFAULT_SERIAL_SPEED = 57600;

    public ProcessingBlipBox(PApplet parent) {
        this.parent = parent;
        if(parent != null)
            parent.registerDispose(this);
    }

    public ProcessingBlipBox(PApplet parent, String serialport) {
        this(parent);
        openSerialPort(serialport);
    }

    public ProcessingBlipBox(PApplet parent, int portindex) {
        this(parent);
        openSerialPort(getSerialPorts().get(portindex));
    }

    public void openSerialPort(int portindex){
        openSerialPort(getSerialPorts().get(portindex), DEFAULT_SERIAL_SPEED);
    }

    public void openSerialPort(String serialport){
        openSerialPort(serialport, DEFAULT_SERIAL_SPEED);
    }

    public void openSerialPort(String serialport, int serialspeed){
        setSerialPort(serialport);
        setSerialSpeed(serialspeed);
        try{
            openSerialPort();
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public void stop(){
        closeSerialPort();
    }

    public int getSensorValue(int index){
        SensorType type = getSensorType(index);
        if(type == null)
            return -1;
        return getSensorValue(type);
    }

    public int getSensorValue(int index, int min, int max){
        SensorType type = getSensorType(index);
        if(type == null)
            return -1;
        return getSensorValue(type, min, max);
    }

    public SensorType getSensorType(int sensorIndex){
        SensorType type = null;
        switch(sensorIndex){
        case 0:
            type = SensorType.X_SENSOR;
            break;
        case 1:
            type = SensorType.Y_SENSOR;
            break;
        case 2:
            type = SensorType.TOUCH_SENSOR;
            break;
        }
        return type;
    }

    public void dispose() {
        closeSerialPort();
    }
}