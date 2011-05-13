package org.blipbox;

public interface BlipBoxInput {
    public void sensorChange(BlipSensor def);
    public void parameterValue(Parameter param, int value);
}
