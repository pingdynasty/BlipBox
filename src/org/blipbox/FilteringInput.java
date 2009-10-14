package org.blipbox;

public class FilteringInput implements BlipBoxInput {
    private BlipBoxInput wrapped;
    private int messageId;

    public void sensorChange(BlipSensor sensor){
        if(sensor.getMessageId() == messageId)
            wrapped.sensorChange(sensor);
    }
    public void parameterValue(int pid, int value){}
}
