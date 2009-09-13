package org.blipbox;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import gnu.io.SerialPortEvent;
import org.apache.log4j.Logger;

import com.pingdynasty.blipbox.SerialDataHandler;

/**
 * Serial event handler responsible for receiving messages from a BlipBox.
 */
public class BlipBoxDataReceiver extends SerialDataHandler {
    private static final Logger log = Logger.getLogger(BlipBoxDataReceiver.class);

    private static final int MESSAGE_ID_MASK    = 0xf << 4;
    private static final int SENSOR_ID_MASK     = 0xff << 2;
    private static final int SENSOR_VALUE_MASK  = 0x3;
//     private static final int SENSOR_FILTER_MASK = 0x03fe; // cut off last bit
    private static final int SENSOR_FILTER_MASK = 0x03ff; // 10 bit value

    private static final int MAX_SENSOR_VALUE = 1023;

    private int[] data = new int[3]; // length of longest message: 3 bytes

    private SensorConfiguration sensors;
    private List<BlipBoxInput> inputhandlers = new ArrayList<BlipBoxInput>();

    public BlipBoxDataReceiver(){
    }

    public void addInputHandler(BlipBoxInput inputhandler){
        inputhandlers.add(inputhandler);
    }

    public void removeInputHandler(BlipBoxInput inputhandler){
        inputhandlers.remove(inputhandler);
    }

    public void sensorChange(SensorDefinition sensor){
        for(BlipBoxInput handler: inputhandlers)
            handler.sensorChange(sensor);
    }

    public void setSensorConfiguration(SensorConfiguration sensors){
        this.sensors = sensors;
    }

    public void serialEvent(SerialPortEvent event) {
        if(event.getEventType()== SerialPortEvent.DATA_AVAILABLE){
            try{
                if(inStream.available() > 0) 
                    readEvent();
            }catch(Throwable exc){
                log.error("Serial io error", exc);
            }
        }
    }
    
    public void readEvent()
        throws IOException {
        data[0] = readSerial();
        SensorDefinition sensor;
        switch(data[0] & MESSAGE_ID_MASK){
        case SensorConfiguration.RELEASE_MSG:
            readReleaseMessage();
            break;
        case SensorConfiguration.XY_MSG:
            readXYMessage();
            break;
        default:
            readSensorMessage();
            break;
        }
    }

    public void readReleaseMessage(){
        SensorDefinition sensor = sensors.getSensorDefinition(SensorType.TOUCH_SENSOR);
        sensor.setValue(MAX_SENSOR_VALUE);
        sensorChange(sensor);
    }

    public void readXYMessage()
        throws IOException {
        // wait for the rest of the data
        while(inStream.available() < 2);
        data[1] = readSerial();
        data[2] = readSerial();
        int xval = (data[0] & 0xf) << 6;
        xval |= data[1] >> 2;
        int yval = (data[1] & 0x3) << 8;
        yval |= data[2];
        SensorDefinition sensor = sensors.getSensorDefinition(SensorType.X_SENSOR);
        sensor.setValue(xval);
        sensorChange(sensor);
        sensor = sensors.getSensorDefinition(SensorType.Y_SENSOR);
        sensor.setValue(yval);
        sensorChange(sensor);
        sensor = sensors.getSensorDefinition(SensorType.TOUCH_SENSOR);
        sensor.setValue(0);
        sensorChange(sensor);
    }

    public SensorDefinition getSensorDefinition(SensorType type){
        return sensors.getSensorDefinition(type);
    }

    public void readSensorMessage()
        throws IOException {
        int messageId = data[0] & SENSOR_ID_MASK;
        SensorDefinition sensor = sensors.getSensorDefinition(messageId);
        if(sensor == null){
            log.warn("Unrecognized sensor message: 0x"+Integer.toHexString(messageId));
        }else{
            // wait for the rest of the data
            while(inStream.available() < 1);
            data[1] = readSerial();
            int value = ((data[0] & SENSOR_VALUE_MASK) << 8) | data[1];
            value &= SENSOR_FILTER_MASK;
            if(value != sensor.getValue()){
                sensor.setValue(value);
                sensorChange(sensor);
            }
        }
    }

    /**
     * try to reset the device by pulling RTS high for 0.2secs
     */
    public void reset(){
        log.debug("Reset");
        serialport.setRTS(true);
        try{
            Thread.sleep(200);
        }catch(Exception exc){}
        serialport.setRTS(false);
    }

}