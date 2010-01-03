package org.blipbox;

import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.io.IOException;
import org.apache.log4j.Logger;

public class BlipBox extends BlipBoxDataSender
    implements BlipBoxProxy, BlipBoxInput {

    private static final Logger log = Logger.getLogger(BlipBox.class);
    private static final int DEFAULT_SERIAL_SPEED = 57600;

    private BlipBoxDataReceiver receiver;
    private String serialport;
    private int serialspeed;
    private int[] leds = new int[16*16];

//     private Map<int, int> sensorValues = new HashMap<int, int>();
    private Map<Integer, Integer> parameterValues = new HashMap<Integer, Integer>();

    public BlipBox() {
        this(new BlipBoxSensorConfiguration());
    }

    public BlipBox(SensorConfiguration config) {
        receiver = new BlipBoxDataReceiver();
        receiver.setSensorConfiguration(config);
        addInputHandler(this);
    }

    public void setSerialSpeed(int serialspeed){
        this.serialspeed = serialspeed;
    }

    public void setSerialPort(String serialport){
        this.serialport = serialport;
    }

    public void openSerialPort(int portindex)
        throws IOException {
        openSerialPort(getSerialPorts().get(portindex), DEFAULT_SERIAL_SPEED);
    }

    public void openSerialPort(int portindex, int serialspeed)
        throws IOException {
        openSerialPort(getSerialPorts().get(portindex), serialspeed);
    }

    public void openSerialPort(String serialport)
        throws IOException {
        openSerialPort(serialport, DEFAULT_SERIAL_SPEED);
    }

    public void openSerialPort(String serialport, int serialspeed)
        throws IOException {
        setSerialPort(serialport);
        setSerialSpeed(serialspeed);
        openSerialPort();
    }

    public void openSerialPort()
        throws IOException {
        receiver.openSerialPort(serialport, serialspeed);
        setOutputStream(receiver.getOutputStream());
    }

    public void closeSerialPort(){
        receiver.closeSerialPort();
    }

    public List<String> getSerialPorts(){
        return receiver.getSerialPorts();
    }

    public void addInputHandler(BlipBoxInput inputhandler){
        receiver.addInputHandler(inputhandler);
    }

    public void addInputHandler(BlipBoxInput inputhandler, SensorType type){
        receiver.addInputHandler(new FilteringInput(inputhandler, type));
        // todo: an input handler added this way cannot be removed with removeInputHandler()
    }

    public void addInputHandler(BlipBoxInput inputhandler, String type){
        BlipSensor sensor = getBlipSensor(type);
        if(sensor == null)
            throw new IllegalArgumentException("No such sensor: "+type);
        receiver.addInputHandler(new FilteringInput(inputhandler, sensor.getMessageId()));
        // todo: an input handler added this way cannot be removed with removeInputHandler()
    }

    public void removeInputHandler(BlipBoxInput inputhandler){
        receiver.removeInputHandler(inputhandler);
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

    public BlipSensor getBlipSensor(SensorType type){
        return receiver.getBlipSensor(type);
    }

    public BlipSensor getBlipSensor(String name){
        return receiver.getBlipSensor(name);
    }

    public void sensorChange(BlipSensor sensor){
        // do naught
//         sensorValues.put(def.getSensorType(), def.getValue())
    }

    public void setParameterValue(int pid, int value){
        parameterValues.put(pid, value);
        setParameter(pid, value);
    }

    public int getParameterValue(int pid){
        return parameterValues.get(pid);
    }

    /* Get the value of the specified sensor as a float in the range [0-1) */
    public float getSensorValue(SensorType type){
        BlipSensor def =  receiver.getBlipSensor(type);
        return def.getValue();
    }

    public float getSensorValue(SensorType type, float min, float max){
        BlipSensor def =  receiver.getBlipSensor(type);
        return def.scale(min, max);
    }

    public int getSensorValue(SensorType type, int min, int max){
        BlipSensor def =  receiver.getBlipSensor(type);
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
     * @return true iff the touch screen is pressed
     */
    public boolean isScreenPressed(){
        return getSensorValue(SensorType.Z_SENSOR) != 0;
    }

    /**
     * Convenience method for getting X sensor value
     */
    public float getX(){
        return getSensorValue(SensorType.X_SENSOR);
    }

    public float getX(float min, float max){
        return getSensorValue(SensorType.X_SENSOR, min, max);
    }

    public int getX(int min, int max){
        return getSensorValue(SensorType.X_SENSOR, min, max);
    }

    /**
     * Convenience method for getting Y sensor value
     */
    public float getY(){
        return getSensorValue(SensorType.Y_SENSOR);
    }

    public float getY(float min, float max){
        return getSensorValue(SensorType.Y_SENSOR, min, max);
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