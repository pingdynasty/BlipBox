package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;

public class SensorConfiguration {

    private Map<Integer, SensorDefinition> sensors = new HashMap<Integer, SensorDefinition>();

    public static final int TOUCH_MIN = 20;
    public static final int TOUCH_MAX = 200;
    public static final int POT_MIN = 252;
    public static final int POT_MAX = 1023;
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

    public void addSensorDefinition(SensorDefinition sensor){
        sensors.put(sensor.messageId, sensor);
    }

    public SensorDefinition getSensorDefinition(int messageId){
        return sensors.get(messageId);
    }

    // todo: MidiConfiguration gets details from SensorConfiguration to set up
    // modes and sensors
    public static SensorConfiguration createSensorConfiguration(String config){
        if("taquito".equals(config))
            return createTaquitoSensorConfiguration();
        else if("blipbox".equals(config))
            return createBlipBoxSensorConfiguration();
        else
            throw new RuntimeException("No such configuration: "+config);
    }

    public static SensorConfiguration createBlipBoxSensorConfiguration(){
        SensorConfiguration sensors = new SensorConfiguration();
        int X_MIN = 160;
        int X_MAX = 860;
        int Y_MIN = 130;
        int Y_MAX = 850;
        sensors.addSensorDefinition(new SensorDefinition(SensorType.TOUCH_SENSOR, TOUCH_SENSOR_MSG, TOUCH_MIN, TOUCH_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.X_SENSOR, X_SENSOR_MSG, X_MIN, X_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.Y_SENSOR, Y_SENSOR_MSG, Y_MIN, Y_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.POT_SENSOR, POT_SENSOR_MSG, POT_MIN, POT_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON1_SENSOR, BUTTON1_SENSOR_MSG, 0, 1));
//         sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON2_SENSOR, BUTTON2_SENSOR_MSG, 0, 1));
//         sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON3_SENSOR, BUTTON3_SENSOR_MSG, 0, 1));
        return sensors;
    }

    public static SensorConfiguration createTaquitoSensorConfiguration(){
        SensorConfiguration sensors = new SensorConfiguration();
        int X_MIN = 40;
        int X_MAX = 1023;
        int Y_MIN = 270;
        int Y_MAX = 880;
        sensors.addSensorDefinition(new SensorDefinition(SensorType.TOUCH_SENSOR, TOUCH_SENSOR_MSG, TOUCH_MIN, TOUCH_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.X_SENSOR, X_SENSOR_MSG, X_MIN, X_MAX));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.Y_SENSOR, Y_SENSOR_MSG, Y_MIN, Y_MAX));
//         sensors.addSensorDefinition(new SensorDefinition(SensorType.POT_SENSOR, POT_SENSOR_MSG, POT_MIN, POT_MAX));
//         sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON1_SENSOR, BUTTON1_SENSOR_MSG, 0, 1));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON2_SENSOR, BUTTON2_SENSOR_MSG, 0, 1));
        sensors.addSensorDefinition(new SensorDefinition(SensorType.BUTTON3_SENSOR, BUTTON3_SENSOR_MSG, 0, 1));
        return sensors;
    }
}