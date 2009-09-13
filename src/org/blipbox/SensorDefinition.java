package org.blipbox;

public class SensorDefinition {
    private SensorType type;
    private int min, max;
    private int messageId;
    private int value;

    public SensorDefinition(SensorType type, int messageId, int min, int max){
        this.type = type;
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

    public SensorType getSensorType(){
        return type;
    }

    public int getMinimumValue(){
        return min;
    }

    public int getMaximumValue(){
        return max;
    }

    public int getMessageId(){
        return messageId;
    }

    public int getValue(){
        return value;
    }

    public void setValue(int value){
        this.value = value;
    }
}
