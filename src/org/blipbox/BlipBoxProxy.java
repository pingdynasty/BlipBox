package org.blipbox;

public interface BlipBoxProxy extends BlipBoxOutput {
    public int getParameterValue(int pid);
    public void setParameterValue(int pid, int value);
    public int getSensorValue(SensorType type);
    public int getSensorValue(SensorType type, int min, int max); // get scaled value
    public boolean isScreenPressed();
    public int getX();
    public int getX(int min, int max);
    public int getY();
    public int getY(int min, int max);
//     public boolean isButtonPressed(int buttonIndex);
}
