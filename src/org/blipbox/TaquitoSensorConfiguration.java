package org.blipbox;

public class TaquitoSensorConfiguration extends SensorConfiguration {

    public static final int Y_MIN = 270;
    public static final int Y_MAX = 880;
    public static final int X_MIN = 40;
    public static final int X_MAX = 1023;
    public static final int TOUCH_MIN = 0;
    public static final int TOUCH_MAX = 1023;
    public static final int POT_MIN = X_MIN;
    public static final int POT_MAX = X_MAX;    

    {
        setBlipSensor(new BlipSensor(SensorType.TOUCH_SENSOR, TOUCH_MIN, TOUCH_MAX));
        setBlipSensor(new BlipSensor(SensorType.X_SENSOR, X_MIN, X_MAX));
        setBlipSensor(new BlipSensor(SensorType.Y_SENSOR, Y_MIN, Y_MAX));
        setBlipSensor(new BlipSensor(SensorType.POT_SENSOR, POT_MIN, POT_MAX));
//         setBlipSensor(new BlipSensor(SensorType.BUTTON1_SENSOR, 0, 1));
        setBlipSensor(new BlipSensor(SensorType.BUTTON2_SENSOR, 0, 1));
        setBlipSensor(new BlipSensor(SensorType.BUTTON3_SENSOR, 0, 1));
    }

}