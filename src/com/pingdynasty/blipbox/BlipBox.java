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
    private static final int SET_FOLLOW_MODE_MESSAGE       = 0x20;
    private static final int SET_LED_MESSAGE               = 0x40;
    private static final int WRITE_CHARACTER_MESSAGE       = 0x50;
    private static final int SHIFT_LEDS_MESSAGE            = 0x60;
    private static final int DISPLAY_EFFECT_MESSAGE        = 0x70;
    private static final int SET_PARAMETER_MESSAGE         = 0x80;
    private static final int BRIGHTNESS_PARAMETER_ID       = 0x04;
    private static final int SENSITIVITY_PARAMETER_ID      = 0x08;

    private OutputStream outStream;

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

    public void setOutputStream(OutputStream outStream){
        this.outStream = outStream;
    }

    public void clear(){
        log.debug("Clear");
        serialWrite(new int[]{CLEAR_MESSAGE});
    }

    protected void sleep(long delay){
        try{
            Thread.sleep(delay);
        }catch(Exception exc){}
    }

    public void sendString(String str, long delay){
        str = str.toLowerCase();
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
        log.error("Unrecognized follow mode: "+mode);
    }

    public void setFollowMode(int mode){
        serialWrite(new int[]{SET_FOLLOW_MODE_MESSAGE|mode});
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
        serialWrite(new int[]{DISPLAY_EFFECT_MESSAGE|effect});
    }

    public void setSensitivity(int level){
        log.debug("Set sensitivity "+level);
        serialWrite(new int[]{SET_PARAMETER_MESSAGE|SENSITIVITY_PARAMETER_ID|(level << 8), level & 0xff});
    }

    public void setBrightness(int level){
        log.debug("Set brightness "+level);
        serialWrite(new int[]{SET_PARAMETER_MESSAGE|BRIGHTNESS_PARAMETER_ID|(level << 8), level & 0xff});
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

    public void serialWrite(int[] data){
        try{
            for(int i=0; i<data.length; ++i)
                outStream.write(data[i]);
            outStream.flush();
        }catch(IOException exc){
            log.error("Serial communication with BlipBox device failed", exc);
        }
    }
}