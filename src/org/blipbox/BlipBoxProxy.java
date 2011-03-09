package org.blipbox;

public interface BlipBoxProxy extends BlipBoxOutput {
    public int getParameterValue(Parameter param);
    public void setParameterValue(Parameter param, int value);
    /* Get the value of the specified sensor as a float in the range [0-1) */
    public float getSensorValue(SensorType type);
    public float getSensorValue(SensorType type, float min, float max); // get scaled value
    public int getSensorValue(SensorType type, int min, int max); // get scaled value
    public boolean isScreenPressed();
    public float getX();
    public float getX(float min, float max);
    public int getX(int min, int max);
    public float getY();
    public float getY(float min, float max);
    public int getY(int min, int max);
    public float getZ();
    public float getZ(float min, float max);
    public int getZ(int min, int max);
//     public boolean isButtonPressed(int buttonIndex);
}
