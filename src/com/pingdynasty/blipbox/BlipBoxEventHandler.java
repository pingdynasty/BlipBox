package com.pingdynasty.blipbox;

public interface BlipBoxEventHandler {

    public enum SensorType { X_SENSOR, Y_SENSOR, T_SENSOR, POT_SENSOR, BUTTON_SENSOR };

    public void sensorChange(SensorType type, int value);
    public void keyDown(byte col, byte row);
    public void keyUp(byte col, byte row);
    public void keyChange(byte oldCol, byte oldRow, byte newCol, byte newRow);

}