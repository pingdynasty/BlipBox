package org.blipbox;

public enum MessageType { 

    XY_MESSAGE (0x5 << 4),
        RELEASE_MESSAGE        (0x7 << 4),
        POT_SENSOR_MESSAGE     (0x80 | (0x4 << 2)),
        BUTTON1_SENSOR_MESSAGE (0x80 | (0x5 << 2)),
        BUTTON2_SENSOR_MESSAGE (0x80 | (0x6 << 2)),
        BUTTON3_SENSOR_MESSAGE (0x80 | (0x7 << 2))
        ;

    private int messageId;

    private MessageType(int messageId){
        this.messageId = messageId;
    }

    public int getMessageId(){
        return messageId;
    }
}
