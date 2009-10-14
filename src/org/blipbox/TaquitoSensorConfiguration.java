package org.blipbox;

public class TaquitoSensorConfiguration extends SensorConfiguration {

    public static final int X_MIN = 40;
    public static final int X_MAX = 1023;
    public static final int Y_MIN = 270;
    public static final int Y_MAX = 880;
    public static final int TOUCH_MIN = 20;
    public static final int TOUCH_MAX = 200;
    public static final int POT_MIN = 252;
    public static final int POT_MAX = 1023;    

    {
        setBlipSensor(new BlipSensor("z", TOUCH_SENSOR_MSG, TOUCH_MIN, TOUCH_MAX));
        setBlipSensor(new BlipSensor("x", X_SENSOR_MSG, X_MIN, X_MAX));
        setBlipSensor(new BlipSensor("y", Y_SENSOR_MSG, Y_MIN, Y_MAX));
//         setBlipSensor(new BlipSensor(SensorType.POT_SENSOR, POT_SENSOR_MSG, POT_MIN, POT_MAX));
//         setBlipSensor(new BlipSensor(SensorType.BUTTON1_SENSOR, BUTTON1_SENSOR_MSG, 0, 1));
        setBlipSensor(new BlipSensor("button1", BUTTON2_SENSOR_MSG, 0, 1));
        setBlipSensor(new BlipSensor("button2", BUTTON3_SENSOR_MSG, 0, 1));
    }

}