package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

public class ProcessingBlipBox extends BlipBox {

    private PApplet parent;

    /**
     * @throws IOException: if an error occurred when opening the serial port
     */
    public ProcessingBlipBox(PApplet parent, String serialport, int serialspeed) {
        super(serialport, serialspeed);
        this.parent = parent;
        parent.registerDispose(this);
        try{
            openSerialPort();
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public ProcessingBlipBox(PApplet parent, int portIndex) {
        this(parent, com.pingdynasty.blipbox.SerialDataHandler.getSerialPorts().get(portIndex), 115200);
    }

    public ProcessingBlipBox(PApplet parent) {
        this(parent, 0);
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