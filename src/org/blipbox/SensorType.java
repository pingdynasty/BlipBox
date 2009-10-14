package org.blipbox;

public enum SensorType { 

        TOUCH_SENSOR       ("z", SensorConfiguration.TOUCH_SENSOR_MSG_ID), 
        X_SENSOR           ("x", SensorConfiguration.X_SENSOR_MSG_ID),
        Y_SENSOR           ("y", SensorConfiguration.Y_SENSOR_MSG_ID), 
        POT_SENSOR         ("analog1", SensorConfiguration.POT_SENSOR_MSG_ID), 
        BUTTON1_SENSOR     ("button1", SensorConfiguration.BUTTON1_SENSOR_MSG_ID),
        BUTTON2_SENSOR     ("button2", SensorConfiguration.BUTTON2_SENSOR_MSG_ID),
        BUTTON3_SENSOR     ("button3", SensorConfiguration.BUTTON3_SENSOR_MSG_ID) ;

    private final String name;
    private final int messageId;

    private SensorType(String name, int messageId){
        this.name = name;
        this.messageId = messageId;
    }

    public String getSensorName(){
        return name;
    }

    public int getMessageId(){
        return messageId;
    }
}
