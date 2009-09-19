package org.blipbox;

import java.util.Map;
import java.util.HashMap;
import java.io.IOException;
import org.apache.log4j.Logger;

public class BlipBox extends BlipBoxDataSender
    implements BlipBoxProxy, BlipBoxInput {

    private static final Logger log = Logger.getLogger(BlipBox.class);

    private BlipBoxDataReceiver receiver;
    private String serialport;
    private int serialspeed;
    private int[] leds = new int[16*16];

//     private Map<int, int> sensorValues = new HashMap<int, int>();
    private Map<Integer, Integer> parameterValues = new HashMap<Integer, Integer>();

    public BlipBox() {
        receiver = new BlipBoxDataReceiver();
        receiver.setSensorConfiguration(SensorConfiguration.createSensorConfiguration("blipbox"));
        addInputHandler(this);
    }

    public BlipBox(String serialport, int serialspeed) {
        this();
        this.serialport = serialport;
        this.serialspeed = serialspeed;
    }

    public void setSerialSpeed(int speed){
        this.serialspeed = serialspeed;
    }

    public void setSerialPort(String port){
        this.serialport = serialport;
    }

    public void addInputHandler(BlipBoxInput inputhandler){
        receiver.addInputHandler(inputhandler);
    }

    public void removeInputHandler(BlipBoxInput inputhandler){
        receiver.removeInputHandler(inputhandler);
    }

    public void openSerialPort()
        throws IOException {
        receiver.openSerialPort(serialport, serialspeed);
        setOutputStream(receiver.getOutputStream());
    }

    public void closeSerialPort(){
        receiver.closeSerialPort();
    }

    public void fade(){
        sendDisplayEffect(5);
        for(int i=0; i<leds.length; ++i)
            leds[i] >>= 1;
    }

    public void brighten(){
        sendDisplayEffect(6);
        for(int i=0; i<leds.length; ++i)
            leds[i] = (leds[i] << 1) | 1;
    }

    public void sensorChange(SensorDefinition def){
        // do naught
//         sensorValues.put(def.getSensorType(), def.getValue())
    }

    public void parameterValue(int pid, int value){
        parameterValues.put(pid, value);
    }

    public int getParameterValue(int pid){
        return parameterValues.get(pid);
    }

    public int getSensorValue(SensorType type){
        SensorDefinition def =  receiver.getSensorDefinition(type);
        return def.getValue();
    }

    public int getSensorValue(SensorType type, int min, int max){
        SensorDefinition def =  receiver.getSensorDefinition(type);
        return def.scale(min, max);
    }

    public void setLed(int index, int value){
        super.setLed(index, value);
        leds[index] = value;
    }

    public void clear(){
        fill(0);
    }

    public void fill(int value){
        super.fill(value);
        for(int i=0; i<leds.length; ++i)
            leds[i] = value;
    }

    public int getLed(int index){
        return leds[index];
    }

    public int getLed(int x, int y){
        return getLed(x*16+y);
    }

    /**
     * Convenience method for getting touch sensor value
     * @return true iff the touch screen is being touched
     */
    public boolean isScreenPressed(){
        return getSensorValue(SensorType.TOUCH_SENSOR) != 0;
    }

    /**
     * Convenience method for getting X sensor value
     */
    public int getX(){
        return getSensorValue(SensorType.X_SENSOR);
    }

    public int getX(int min, int max){
        return getSensorValue(SensorType.X_SENSOR, min, max);
    }

    /**
     * Convenience method for getting Y sensor value
     */
    public int getY(){
        return getSensorValue(SensorType.Y_SENSOR);
    }

    public int getY(int min, int max){
        return getSensorValue(SensorType.Y_SENSOR, min, max);
    }

//     public boolean isButtonPressed(int buttonIndex){
//         // todo!
//         // remove?
//         return false;
//     }

}