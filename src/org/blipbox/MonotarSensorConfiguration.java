package org.blipbox;

public class MonotarSensorConfiguration extends SensorConfiguration {

    public static final int Y_MIN = 160;
    public static final int Y_MAX = 860;
    public static final int X_MIN = 140;
    public static final int X_MAX = 830;
    public static final int Z_MIN = 0;
    public static final int Z_MAX = 1023;
    public static final int POT_MIN = 0;
    public static final int POT_MAX = 1023;    

    {
        setBlipSensor(new BlipSensor(SensorType.Z_SENSOR, Z_MIN, Z_MAX));
        setBlipSensor(new BlipSensor(SensorType.X_SENSOR, X_MIN, X_MAX));
        setBlipSensor(new BlipSensor(SensorType.Y_SENSOR, Y_MIN, Y_MAX));
        setBlipSensor(new BlipSensor(SensorType.POT_SENSOR, POT_MIN, POT_MAX));
    }

}