package org.blipbox;

public enum SensorType { 

        TOUCH_SENSOR       ("z", 0x80 | (0x1 << 2)), 
        X_SENSOR           ("x", 0x80 | (0x2 << 2)),
        Y_SENSOR           ("y", 0x80 | (0x3 << 2)), 
        POT_SENSOR         ("analog1", 0x80 | (0x4 << 2)), 
        BUTTON1_SENSOR     ("button1", 0x80 | (0x5 << 2)),
        BUTTON2_SENSOR     ("button2", 0x80 | (0x6 << 2)),
        BUTTON3_SENSOR     ("button3", 0x80 | (0x7 << 2)) ;

//     public static final int XY_MSG_ID             = 0x5 << 4;
//     public static final int RELEASE_MSG_ID        = 0x7 << 4;
//     public static final int TOUCH_SENSOR_MSG_ID   = 0x80 | (0x1 << 2);
//     public static final int X_SENSOR_MSG_ID       = 0x80 | (0x2 << 2);
//     public static final int Y_SENSOR_MSG_ID       = 0x80 | (0x3 << 2);
//     public static final int POT_SENSOR_MSG_ID     = 0x80 | (0x4 << 2);
//     public static final int BUTTON1_SENSOR_MSG_ID = 0x80 | (0x5 << 2);
//     public static final int BUTTON2_SENSOR_MSG_ID = 0x80 | (0x6 << 2);
//     public static final int BUTTON3_SENSOR_MSG_ID = 0x80 | (0x7 << 2);
//     public static final int PING_SENSOR_MSG_ID =    0x80 | (0x8 << 2);

    private String name;
    private int messageId;

    SensorType(String name, int messageId){
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
