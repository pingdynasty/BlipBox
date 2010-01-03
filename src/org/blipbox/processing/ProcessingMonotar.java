package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

public class ProcessingMonotar extends ProcessingBlipBox {

    public ProcessingMonotar(PApplet parent) {
        super(new MonotarSensorConfiguration(), parent);
    }

    public ProcessingMonotar(PApplet parent, String serialport) {
        super(new MonotarSensorConfiguration(), parent);
        try{
            openSerialPort(serialport);
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public ProcessingMonotar(PApplet parent, int portindex) {
        super(new MonotarSensorConfiguration(), parent);
        try{
            openSerialPort(getSerialPorts().get(portindex));
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

}