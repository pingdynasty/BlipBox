package org.blipbox;

public interface BlipBoxInput {
    public void sensorChange(SensorDefinition def);
    public void parameterValue(int pid, int value);
}
