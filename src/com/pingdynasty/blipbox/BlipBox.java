package com.pingdynasty.blipbox;

import java.io.IOException;
import java.io.OutputStream;
import org.apache.log4j.Logger;

/**
 * Class responsible for routing messages to a BlipBox.
 * Provides an interface to BlipBox commands and functionality.
 */
public class BlipBox {
    private static final Logger log = Logger.getLogger(BlipBox.class);

    private static final int CLEAR_MESSAGE                 = 0x10;

    private static final int SET_LED_MESSAGE               = 0x20;
    private static final int SET_LED_ROW_MESSAGE           = 0x30;
    private static final int SET_LED_COL_MESSAGE           = 0x40;

    private static final int WRITE_CHARACTER_MESSAGE       = 0x50;
    private static final int SHIFT_LEDS_MESSAGE            = 0x60;
    private static final int DISPLAY_EFFECT_MESSAGE        = 0x70;

    private static final int SET_PARAMETER_MESSAGE         = 0xc0;

    private static final int BRIGHTNESS_PARAMETER_ID      = (0x01 << 2);
    private static final int SENSITIVITY_PARAMETER_ID     = (0x02 << 2);
    private static final int FOLLOW_MODE_PARAMETER_ID     = (0x03 << 2);
    private static final int X_MIN_PARAMETER_ID           = (0x04 << 2);
    private static final int X_RANGE_PARAMETER_ID         = (0x05 << 2);
    private static final int Y_MIN_PARAMETER_ID           = (0x06 << 2);
    private static final int Y_RANGE_PARAMETER_ID         = (0x07 << 2);
    private static final int SERIAL_SPEED_PARAMETER_ID    = (0x08 << 2);

    private OutputStream outStream;
    private long timestamp;
    private long frequency = 0;

    private String[] followModes = new String[]{
        "None", "Dot", "Cross", "Criss", "Star", "Blob", "Square"
    };

    private String[] displayEffects = new String[]{
        "Greeting", "Rain", "Bounce", "Fade", "Explode"
    };

    /**
     * @param outStream: serial line connected to a BlipBox
     */
    public BlipBox(){
    }

    public int getMessageFrequency(){
        return (int)frequency;
    }

    public void setMessageFrequency(int frequency){
        this.frequency = frequency;
    }

    public void setOutputStream(OutputStream outStream){
        this.outStream = outStream;
    }

    /**
     * clear all LEDs
     */
    public void clear(){
        log.debug("Clear");
        serialWrite(new int[]{CLEAR_MESSAGE});
    }

    /**
     * opposite of clear - sets all LEDs
     */
    public void flush(){
        log.debug("Flush");
//         for(int i=0; i<8; ++i)
//             setLedRow(i, 0xff);
        for(int i=0; i<10; ++i)
            setLedColumn(i, 0xff);
    }

    protected void sleep(long delay){
        try{
            Thread.sleep(delay);
        }catch(Exception exc){}
    }

    public void sendString(String str, long delay){
        for(char c: str.toCharArray()){
            shiftLeft(1);
            sleep(delay);
            for(int pos=9; pos>4; --pos){
                shiftLeft(1);
                sendCharacter(pos, c);
                sleep(delay);
            }
        }
    }

    public void sendCharacter(int pos, char c){
        log.debug("Sending character "+c+" at pos "+pos);
        serialWrite(new int[]{WRITE_CHARACTER_MESSAGE | (pos & 0x0f), (int)c});
    }

    public void shift(int direction, int steps){
        serialWrite(new int[]{SHIFT_LEDS_MESSAGE | ((direction & 0x3) << 2) | (steps & 0x3)});
    }

    public void shiftLeft(int steps){
        log.debug("Shift left");
        shift(2, steps);
    }

    public void shiftRight(int steps){
        log.debug("Shift right");
        shift(3, steps);
    }

    public void shiftUp(int steps){
        log.debug("Shift up");
        shift(0, steps);
    }

    public void shiftDown(int steps){
        log.debug("Shift down");
        shift(1, steps);
    }

    public String[] getDisplayEffects(){
        return displayEffects;
    }

    public void sendDisplayEffect(String effect){
        log.debug("Send display effect "+effect);
        for(int i=0; i<displayEffects.length; ++i){
            if(displayEffects[i].equals(effect)){
                sendDisplayEffect(i);
                return;
            }
        }
        log.error("Unrecognized display effect: "+effect);
    }

    public void sendDisplayEffect(int effect){
        serialWrite(new int[]{DISPLAY_EFFECT_MESSAGE | (effect & 0x0f)});
    }

    public String[] getFollowModes(){
        return followModes;
    }

    public void setFollowMode(String mode){
        log.debug("Set follow mode "+mode);
        for(int i=0; i<followModes.length; ++i){
            if(followModes[i].equals(mode)){
                setFollowMode(i);
                return;
            }
        }
        throw new IllegalArgumentException("Unrecognized follow mode: "+mode);
    }

    public void setFollowMode(int mode){
        setParameter(FOLLOW_MODE_PARAMETER_ID, mode);
    }

    public void setSensitivity(int level){
        log.debug("Set sensitivity "+level);
        setParameter(SENSITIVITY_PARAMETER_ID, level);
    }

    public void setBrightness(int level){
        log.debug("Set brightness "+level);
        setParameter(BRIGHTNESS_PARAMETER_ID, level);
    }

    /**
     * set serial speed e.g. 9600, 57600
     * value must be a multiple of 9600
     */
    public void setSerialSpeed(long speed){
        log.debug("Set serial speed to "+speed);
        if(speed % 9600 != 0)
            throw new IllegalArgumentException("Serial speed must be a multiple of 9600");
        setParameter(SERIAL_SPEED_PARAMETER_ID, (int)(speed / 9600));
    }

    public void setParameter(int pid, int value){
        log.debug("Set parameter id "+pid+" to value "+value);
        // set parameter message: 11ppppvv vvvvvvvv
        // 0x3c is 00111100, same as 0xf<<2
        if((pid & 0xc3) != 0)
            throw new IllegalArgumentException("Illegal parameter id: "+pid);
        serialWrite(new int[]{SET_PARAMETER_MESSAGE | pid | (value >> 8 & 0x3), value & 0xff});
    }

    public void setLed(int x, int y, int value){
        setLed(x*16+y, value);
    }

    public void setLed(int led, int value){
        log.debug("Set led "+led+" to "+value);
        if(((led | value) & 0xff00) != 0)
            throw new IllegalArgumentException("Invalid LED index or brightness: "+led+"/"+value);
        serialWrite(new int[]{SET_LED_MESSAGE, led, value});
    }

    public void setLedColumn(int col, int data){
        log.debug("Set led column "+col+" to "+data);
        if(col < 10) // todo: remove restriction or make the size configurable
            serialWrite(new int[]{SET_LED_COL_MESSAGE | (col & 0x0f), data});
    }

    public void setLedRow(int row, int data){
        log.debug("Set led row "+row+" to "+data);
        if(row < 8) // todo: remove restriction or make the size configurable
            serialWrite(new int[]{SET_LED_ROW_MESSAGE | (row & 0x0f), data});
    }

    public void serialWrite(int[] data){
        long now = System.currentTimeMillis();
        while(timestamp + frequency > now){
            log.debug("Stalling "+(timestamp + frequency - now)+"ms");
            try{
                Thread.sleep(timestamp + frequency - now);
            }catch(InterruptedException exc){
                log.error(exc);
            }
            now = System.currentTimeMillis();
        }
        timestamp = System.currentTimeMillis();
        try{
            for(int i=0; i<data.length; ++i)
                outStream.write(data[i]);
            outStream.flush();
        }catch(IOException exc){
            log.error("Serial communication with BlipBox device failed", exc);
        }
    }
}