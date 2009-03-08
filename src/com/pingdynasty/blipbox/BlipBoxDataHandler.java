package com.pingdynasty.blipbox;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import gnu.io.SerialPortEvent;
import org.apache.log4j.Logger;

public class BlipBoxDataHandler extends SerialDataHandler {
    private static final Logger log = Logger.getLogger(BlipBoxDataHandler.class);

    private static final int MESSAGE_ID_MASK    = 0xf << 4;
    private static final int SENSOR_ID_MASK     = 0xff << 2;
    private static final int SENSOR_VALUE_MASK  = 0x3;
//     private static final int SENSOR_FILTER_MASK = 0x03fe; // cut off last bit
    private static final int SENSOR_FILTER_MASK = 0x03ff; // 10 bit value

    private static final int CLEAR_MESSAGE                 = 0x10;
    private static final int SET_FOLLOW_MODE_MESSAGE       = 0x20;
    private static final int SET_SENSITIVITY_MESSAGE       = 0x30;
    private static final int SET_LED_MESSAGE               = 0x40;

    private int[] data = new int[3]; // length of longest message: 3 bytes

    private SensorConfiguration sensors;
    private SensorEventHandler eventhandler;

    private String[] followModes = new String[]{
        "None", "Dot", "Cross", "Criss", "Star", "Blob", "Square"
    };

    public BlipBoxDataHandler(String config){
        eventhandler = new LoggingEventHandler();
        sensors = SensorConfiguration.createSensorConfiguration(config);
    }

    public MidiOutputEventHandler createMidiOutput(){
        eventhandler = new MidiOutputEventHandler();
        return (MidiOutputEventHandler)eventhandler;
    }

    public void createGraph(){
        // todo: fix grapher to use new i/f
//         eventhandler = new GraphingEventHandler();
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
        SensorDefinition sensor = sensors.getSensorDefinition(SensorConfiguration.TOUCH_SENSOR_MSG);
        sensor.value = 1023;
        eventhandler.sensorChange(sensor);
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
        SensorDefinition sensor = sensors.getSensorDefinition(SensorConfiguration.X_SENSOR_MSG);
        sensor.value = xval;
        eventhandler.sensorChange(sensor);
        sensor = sensors.getSensorDefinition(SensorConfiguration.Y_SENSOR_MSG);
        sensor.value = yval;
        eventhandler.sensorChange(sensor);
        sensor = sensors.getSensorDefinition(SensorConfiguration.TOUCH_SENSOR_MSG);
        sensor.value = 0;
        eventhandler.sensorChange(sensor);
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
            if(value != sensor.value){
                sensor.value = value;
                eventhandler.sensorChange(sensor);
            }
        }
    }

    public void clear(){
        try{
            outStream.write(CLEAR_MESSAGE);
        }catch(Exception exc){
            log.error("Failed to send clear message", exc);
        }
    }

    public String[] getFollowModes(){
        return followModes;
    }

    public void setFollowMode(String mode){
        for(int i=0; i<followModes.length; ++i){
            if(followModes[i].equals(mode)){
                setFollowMode(i);
                return;
            }
        }
        log.error("Unrecognized follow mode: "+mode);
    }

    public void setFollowMode(int mode){
        try{
            outStream.write(SET_FOLLOW_MODE_MESSAGE|mode);
            outStream.flush();
        }catch(Exception exc){
            log.error("Failed to set follow mode", exc);
        }
    }

    public void setSensitivity(int level){
        try{
            outStream.write(SET_SENSITIVITY_MESSAGE|(level << 8));
            outStream.write(level & 0xff);
            outStream.flush();
        }catch(Exception exc){
            log.error("Failed to set follow mode", exc);
        }
    }

    public void setLed(int x, int y, int value){
        setLed(x*16+y, value);
    }

    public void setLed(int led, int value){
        if(((led | value) & 0xff00) != 0)
            throw new IllegalArgumentException("Invalid LED index or brightness: "+led+"/"+value);
        try{
            outStream.write(SET_LED_MESSAGE);
            outStream.write(led);
            outStream.write(value);
            // hold on... does the cast to byte turn it into a signed value?
//             outStream.write(new byte[]{SET_LED_MESSAGE, (byte)led, (byte)value});
            outStream.flush();
        }catch(Exception exc){
            log.error("Failed to set led", exc);
        }
    }

}