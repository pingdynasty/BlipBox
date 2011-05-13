package org.blipbox;

import java.io.OutputStream;
import java.io.IOException;
import org.apache.log4j.Logger;

public class BlipBoxDataSender implements BlipBoxOutput {

    private static final Logger log = Logger.getLogger(BlipBoxDataSender.class);

    private static final int MAX_ROW = 8;
    private static final int MAX_COL = 10;

    private long timestamp;
    private long frequency = 0;
    private OutputStream serialOutput;

    private static final int CLEAR_MESSAGE                 = 0x10;

    private static final int SET_LED_MESSAGE               = 0x20;
    private static final int SET_LED_ROW_MESSAGE           = 0x30;
    private static final int SET_LED_COL_MESSAGE           = 0x40;

    private static final int WRITE_CHARACTER_MESSAGE       = 0x50;
    private static final int SHIFT_LEDS_MESSAGE            = 0x60;
    private static final int COMMAND_MESSAGE               = 0x70;

    private static final int SET_PARAMETER_MESSAGE         = 0xc0;

//     private static final int BRIGHTNESS_PARAMETER_ID      = (0x01 << 2);
//     private static final int SENSITIVITY_PARAMETER_ID     = (0x02 << 2);
// //     private static final int FOLLOW_MODE_PARAMETER_ID     = (0x03 << 2);
//     private static final int TLC_GSCLK_PERIOD_PARAMETER_ID= (0x03 << 2);
//     private static final int SERIAL_SPEED_PARAMETER_ID    = (0x04 << 2);
// //     private static final int X_MIN_PARAMETER_ID           = (0x05 << 2);
// //     private static final int X_RANGE_PARAMETER_ID         = (0x06 << 2);
// //     private static final int Y_MIN_PARAMETER_ID           = (0x07 << 2);
// //     private static final int Y_RANGE_PARAMETER_ID         = (0x08 << 2);

    protected void sleep(long delay){
        try{
            Thread.sleep(delay);
        }catch(InterruptedException exc){
            log.error(exc);
        }
    }

    public void setOutputStream(OutputStream serialOutput){
        this.serialOutput = serialOutput;
    }

    public void serialWrite(int[] data){
// 	for(int i=0; i<data.length; ++i)
// 	    System.out.print("\t 0x"+Integer.toHexString(data[i]));
// 	System.out.println();
        long now = System.currentTimeMillis();
        while(timestamp + frequency > now){
//             log.debug("Stalling "+(timestamp + frequency - now)+"ms");
            sleep(timestamp + frequency - now);
            now = System.currentTimeMillis();
        }
        timestamp = System.currentTimeMillis();
        try{
            for(int i=0; i<data.length; ++i)
                serialOutput.write(data[i]);
            serialOutput.flush();
        }catch(IOException exc){
            log.error("Serial communication with BlipBox device failed", exc);
        }
    }

    public void setLed(int x, int y, int value){
        log.debug("Set led "+x+"/"+y+" to "+value);
        setLed(x*16+y, value);
    }

    public void setLed(int led, int value){
//         log.debug("Set led "+led+" to "+value);
        if(((led | value) & 0xff00) != 0)
            throw new IllegalArgumentException("Invalid LED index or brightness: "+led+"/"+value);
        serialWrite(new int[]{SET_LED_MESSAGE, led, value});
    }

    public void setLedColumn(int col, int data){
//         log.debug("Set led column "+col+" to "+data);
        if(col >= MAX_COL)
            throw new IllegalArgumentException("Column index out of bounds: "+col);
        serialWrite(new int[]{SET_LED_COL_MESSAGE | (col & 0x0f), data});
    }

    public void setLedRow(int row, int data){
//         log.debug("Set led row "+row+" to "+data);
        if(row >= MAX_ROW)
            throw new IllegalArgumentException("Row index out of bounds: "+row);
        serialWrite(new int[]{SET_LED_ROW_MESSAGE | (row & 0x0f), data});
    }

    /**
     * clear all LEDs
     */
    public void clear(){
//         log.debug("Clear");
        serialWrite(new int[]{CLEAR_MESSAGE});
    }

    /**
     * set value of all LEDs
     * @param brightness in the range 0-255. Note that this value will be scaled down to four bits.
     */
    public void fill(int brightness){
//         log.debug("Fill with "+brightness);
        if(brightness > 0xff || brightness < 0)
            throw new IllegalArgumentException("Fill value out of bounds: "+brightness);
        brightness /= 0x11;
        serialWrite(new int[]{CLEAR_MESSAGE|brightness});
    }

    public void writeText(String str, long delay){
        for(char c: str.toCharArray()){
            shiftHorizontal(-1);
            sleep(delay);
            // currently this writes backwards!
            for(int pos=9; pos>4; --pos){
                shiftLeft(1);
                writeCharacter(pos, c);
                sleep(delay);
            }
        }
    }

    public void writeCharacter(int pos, char c){
//         log.debug("Sending character "+c+" at pos "+pos);
        serialWrite(new int[]{WRITE_CHARACTER_MESSAGE | (pos & 0x0f), (int)c});
    }

    public void shift(int direction, int steps){
        serialWrite(new int[]{SHIFT_LEDS_MESSAGE | ((direction & 0x3) << 2) | (steps & 0x3)});
    }

    public void shiftHorizontal(int steps){
        if(steps < 0)
            shiftLeft(steps*-1);
        else
            shiftRight(steps);
    }

    public void shiftLeft(int steps){
//         log.debug("Shift left "+steps+" steps");
        shift(2, steps);
    }

    public void shiftRight(int steps){
//         log.debug("Shift right "+steps+" steps");
        shift(3, steps);
    }

    public void shiftVertical(int steps){
        if(steps < 0)
            shiftDown(steps*-1);
        else
            shiftUp(steps);
    }

    public void shiftUp(int steps){
//         log.debug("Shift up "+steps+" steps");
        shift(0, steps);
    }

    public void shiftDown(int steps){
//         log.debug("Shift down "+steps+" steps");
        shift(1, steps);
    }

    public void sendCommand(Command cmd){
        serialWrite(new int[]{COMMAND_MESSAGE | cmd.cid});
    }

    public void setParameterValue(Parameter param, int value){
        log.debug("Set parameter id "+param+" to value "+value);
        setParameter(param.pid, value);
    }

    private void setParameter(int pid, int value){
//         log.debug("Set parameter id "+pid+" to value "+value);
        // set parameter message: 11ppppvv vvvvvvvv
        // 0x3c is 00111100, same as 0xf<<2
        if((pid & 0xc3) != 0)
            throw new IllegalArgumentException("Illegal parameter id: "+pid);
        serialWrite(new int[]{SET_PARAMETER_MESSAGE | pid | (value >> 8 & 0x3), value & 0xff});
    }

}