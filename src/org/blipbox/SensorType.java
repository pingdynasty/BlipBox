package org.blipbox;

public enum SensorType { 

        TOUCH_SENSOR       (0x80 | (0x1 << 2)), 
        X_SENSOR           (0x80 | (0x2 << 2)),
        Y_SENSOR           (0x80 | (0x3 << 2)), 
        POT_SENSOR         (0x80 | (0x4 << 2)), 
        BUTTON1_SENSOR     (0x80 | (0x5 << 2)),
        BUTTON2_SENSOR     (0x80 | (0x6 << 2)),
        BUTTON3_SENSOR     (0x80 | (0x7 << 2)) ;

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

    private int messageId;

    private SensorType(int messageId){
        this.messageId = messageId;
    }

    public int getMessageId(){
        return messageId;
    }
}
