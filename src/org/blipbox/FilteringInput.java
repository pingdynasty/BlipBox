package org.blipbox;

public class FilteringInput implements BlipBoxInput {
    private BlipBoxInput wrapped;
    private int messageId;

    public FilteringInput(BlipBoxInput wrapped, SensorType type){
        this.wrapped = wrapped;
        this.messageId = type.getMessageId();
    }

    public FilteringInput(BlipBoxInput wrapped, int messageId){
        this.wrapped = wrapped;
        this.messageId = messageId;
    }

    public void sensorChange(BlipSensor sensor){
        if(sensor.getMessageId() == messageId)
            wrapped.sensorChange(sensor);
    }

    public void parameterValue(Parameter param, int value){}
}
