package org.blipbox.processing;

import java.io.IOException;
import processing.core.*;
import org.blipbox.*;

public class ProcessingTaquito extends ProcessingBlipBox {

    public ProcessingTaquito(PApplet parent) {
        super(new TaquitoSensorConfiguration(), parent);
    }

    public ProcessingTaquito(PApplet parent, String serialport) {
        super(new TaquitoSensorConfiguration(), parent);
        try{
            openSerialPort(serialport);
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

    public ProcessingTaquito(PApplet parent, int portindex) {
        super(new TaquitoSensorConfiguration(), parent);
        try{
            openSerialPort(getSerialPorts().get(portindex));
        }catch(IOException exc){
            throw new RuntimeException(exc);
        }
    }

}