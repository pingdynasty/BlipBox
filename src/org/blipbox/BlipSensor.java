package org.blipbox;

public class BlipSensor {

    private String name;
    private int messageId;
    private int min, max;
    private int value;

    public BlipSensor(SensorType type, int min, int max){
        this.name = type.getSensorName();
        this.messageId = type.getMessageId();
        this.min = min;
        this.max = max;
    }

    public BlipSensor(String name, int messageId, int min, int max){
        this.name = name;
        this.messageId = messageId;
        this.min = min;
        this.max = max;
    }

    /** scale a value to the range [0,range)
     */
    public int scale(int range){
        return scale(0, range);
    }

    /** scale a value to the range [toMin,toMax)
     */
    public int scale(int toMin, int toMax){
        if(value < min)
            return toMin;
        if(value >= max)
            return toMax-1;
        return toMin + (value - min) * (toMax - toMin) / (max - min);
    }

    public String getSensorName(){
        return name;
    }

    public int getMinimumValue(){
        return min;
    }

    public int getMaximumValue(){
        return max;
    }

    public void setMinimumValue(int min){
        this.min = min;
    }

    public void setMaximumValue(int max){
        this.max = max;
    }

    public int getMessageId(){
        return messageId;
    }

    public int getRawValue(){
        return value;
    }

    /* Get the sensor value as a float in the range [0-1) */
    public float getValue(){
        return scale(0, 10000)/10000.0f;
    }

    public float scale(float toMin, float toMax){
        if(value < min)
            return toMin;
        if(value >= max)
            return toMax-1;
        return toMin + (value - min) * (toMax - toMin) / (max - min);
    }

    public void setValue(int value){
        this.value = value;
    }
}
