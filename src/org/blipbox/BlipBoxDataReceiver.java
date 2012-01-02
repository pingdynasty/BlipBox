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
    public static final int MAX_SENSOR_VALUE = 1023;

    private int[] data = new int[3]; // length of longest message: 3 bytes
    private List<BlipBoxInput> inputhandlers = new ArrayList<BlipBoxInput>();
    private SensorConfiguration sensors;

    public BlipBoxDataReceiver(){
    }

    public void addInputHandler(BlipBoxInput inputhandler){
        inputhandlers.add(inputhandler);
    }

    public void removeInputHandler(BlipBoxInput inputhandler){
        inputhandlers.remove(inputhandler);
    }

    public void sensorChange(BlipSensor sensor){
        for(BlipBoxInput handler: inputhandlers)
            handler.sensorChange(sensor);
    }

    public void parameterValue(Parameter param, int value){
        for(BlipBoxInput handler: inputhandlers)
            handler.parameterValue(param, value);
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
// 	System.out.println("rx 0x"+Integer.toHexString(data[0]));
	if((data[0] & MESSAGE_ID_MASK) == SensorConfiguration.POSITION_MSG_ID){
            readPositionMessage();
	}else if((data[0] & MESSAGE_ID_MASK) == SensorConfiguration.RELEASE_MSG_ID){
            readReleaseMessage();
	}else if((data[0] & 0xc0) == 0x80){
            readSensorMessage();
	}else if((data[0] & 0xc0) == 0xc0){
           readParameterMessage();
	}else{
	    log.error("Unrecognized message byte 0x"+Integer.toHexString(data[0]));
        }
    }

    static final int PARAMETER_ID_MASK = 0x3c; // 00111100
    static final int PARAMETER_VALUE_MASK = 0x3ff; // 00000011 11111111
    public void readParameterMessage()
        throws IOException {
        // wait for the rest of the data
        while(inStream.available() < 1);
        data[1] = readSerial();
        int pid = data[0] & PARAMETER_ID_MASK;
        Parameter param = Parameter.getParameterForId(pid);
        if(param == null)
            log.error("Unrecognized parameter id: 0x"+Integer.toHexString(pid));
        else
            parameterValue(param, (data[0]<<8|data[1])&PARAMETER_VALUE_MASK);
    }

    public void readReleaseMessage(){
        BlipSensor sensor = sensors.getBlipSensor(SensorType.Z_SENSOR);
        sensor.setValue(0);
        sensorChange(sensor);
    }

    public void readPositionMessage()
        throws IOException {
        // wait for the rest of the data
        while(inStream.available() < 2);
        data[1] = readSerial();
        data[2] = readSerial();
        int xval = (data[0] & 0xf) << 6;
        xval |= data[1] >> 2;
        int yval = (data[1] & 0x3) << 8;
        yval |= data[2];
        BlipSensor sensor = sensors.getBlipSensor(SensorType.X_SENSOR);
        sensor.setValue(xval);
        sensorChange(sensor);
        sensor = sensors.getBlipSensor(SensorType.Y_SENSOR);
        sensor.setValue(yval);
        sensorChange(sensor);
        sensor = sensors.getBlipSensor(SensorType.Z_SENSOR);
        sensor.setValue(MAX_SENSOR_VALUE);
        sensorChange(sensor);
    }

    public BlipSensor getBlipSensor(SensorType type){
        return sensors.getBlipSensor(type);
    }

    public BlipSensor getBlipSensor(String name){
        return sensors.getBlipSensor(name);
    }

    public void readSensorMessage()
        throws IOException {
        int messageId = data[0] & SENSOR_ID_MASK;
        BlipSensor sensor = sensors.getBlipSensor(messageId);
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