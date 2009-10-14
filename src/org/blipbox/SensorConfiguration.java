package org.blipbox;

import java.util.Map;
import java.util.HashMap;

public class SensorConfiguration {

    private Map<String, BlipSensor> sensorsByName = new HashMap<String, BlipSensor>();
    private Map<Integer, BlipSensor> sensorsById = new HashMap<Integer, BlipSensor>();

    public static final int XY_MSG             = 0x5 << 4;
    public static final int RELEASE_MSG        = 0x7 << 4;
    public static final int TOUCH_SENSOR_MSG   = 0x80 | (0x1 << 2);
    public static final int X_SENSOR_MSG       = 0x80 | (0x2 << 2);
    public static final int Y_SENSOR_MSG       = 0x80 | (0x3 << 2);
    public static final int POT_SENSOR_MSG     = 0x80 | (0x4 << 2);
    public static final int BUTTON1_SENSOR_MSG = 0x80 | (0x5 << 2);
    public static final int BUTTON2_SENSOR_MSG = 0x80 | (0x6 << 2);
    public static final int BUTTON3_SENSOR_MSG = 0x80 | (0x7 << 2);
    public static final int PING_SENSOR_MSG =    0x80 | (0x8 << 2);

    public void setBlipSensor(BlipSensor sensor){
        sensorsByName.put(sensor.getSensorName(), sensor);
        sensorsById.put(sensor.getMessageId(), sensor);
    }

    public BlipSensor getBlipSensor(String name){
        return sensorsByName.get(name);
    }

    public BlipSensor getBlipSensor(int messageId){
        return sensorsById.get(messageId);
    }

    public BlipSensor getBlipSensor(SensorType type){
        return sensorsById.get(type.getMessageId());
    }

    // todo: MidiConfiguration gets details from SensorConfiguration to set up
    // modes and sensors
    public static SensorConfiguration createSensorConfiguration(String config){
        if("taquito".equalsIgnoreCase(config))
            return new TaquitoSensorConfiguration();
        else if("blipbox".equalsIgnoreCase(config))
            return new BlipBoxSensorConfiguration();
        else
            throw new IllegalArgumentException("No such configuration: "+config);
    }
}