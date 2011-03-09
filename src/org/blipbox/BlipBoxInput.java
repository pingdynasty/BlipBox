package org.blipbox;

public interface BlipBoxInput {
    public void sensorChange(BlipSensor def);
    // todo: this method has to actually be implemented in BlipBoxDataReceiver
    public void parameterValue(Parameter param, int value);
}
