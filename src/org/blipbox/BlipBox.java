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
    private static final int BRIGHTNESS_STEP = 4;

    private BlipBoxDataReceiver receiver;
    private String serialport;
    private int serialspeed = DEFAULT_SERIAL_SPEED;
    private int[] leds = new int[16*16];

//     private Map<int, int> sensorValues = new HashMap<int, int>();
    private Map<Parameter, Integer> parameterValues = new HashMap<Parameter, Integer>();

    public BlipBox() {
        this(new BlipBoxSensorConfiguration());
    }

    public BlipBox(SensorConfiguration config) {
        receiver = new BlipBoxDataReceiver();
        receiver.setSensorConfiguration(config);
        addInputHandler(this);
    }

    public int getWidth(){
	return 10;
    }

    public int getHeight(){
	return 8;
    }

    public void setSerialSpeed(int serialspeed){
        this.serialspeed = serialspeed;
    }

    public void setSerialPort(String serialport){
        this.serialport = serialport;
    }

    public void openSerialPort(int portindex)
        throws IOException {
        openSerialPort(getSerialPorts().get(portindex), serialspeed);
    }

    public void openSerialPort(int portindex, int serialspeed)
        throws IOException {
        openSerialPort(getSerialPorts().get(portindex), serialspeed);
    }

    public void openSerialPort(String serialport)
        throws IOException {
        setSerialPort(serialport);
        openSerialPort();
    }

    public void openSerialPort(String serialport, int serialspeed)
        throws IOException {
        setSerialPort(serialport);
        setSerialSpeed(serialspeed);
        openSerialPort();
    }

    public void openSerialPort()
        throws IOException {
// 	log.info("Opening serial port "+serialport+" at "+serialspeed+" baud");
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

    public void sendCommand(Command cmd){
        switch(cmd){
        case FADE:
            fade();
            break;
        case BRIGHTEN:
            brighten();
            break;
        default:
            super.sendCommand(cmd);
        }
    }

    public void fade(){
        super.sendCommand(Command.FADE);
        for(int i=0; i<leds.length; ++i)
            leds[i] = Math.max(leds[i]-BRIGHTNESS_STEP, 0);
//             leds[i] >>= 1;
    }

    public void brighten(){
        super.sendCommand(Command.BRIGHTEN);
        for(int i=0; i<leds.length; ++i)
            leds[i] = Math.min(leds[i]+BRIGHTNESS_STEP, 0xff);
//             leds[i] = (leds[i] << 1) | 1;
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

    // called by device
    public void parameterValue(Parameter param, int value){
        parameterValues.put(param, value);
        log.info("parameter "+param+": "+value);
    }

    // sends parameter update to device
    public void setParameterValue(Parameter param, int value){
        super.setParameterValue(param, value);
        parameterValues.put(param, value);
    }

    // get current value from proxy
    public int getParameterValue(Parameter param){
        if(parameterValues.containsKey(param))
            return parameterValues.get(param);
        return 0;
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

    public void shift(int direction, int steps){
        super.shift(direction, steps);
        // the leftmost 2 bits determine the direction: 0: left, 1: right, 2: up, 3: down
        // the rightmost 2 bits determines the number of steps: 1-4
        for(int i=0; i<steps; ++i){
            switch(direction){
            case 0x1: // shift left
                for(int col=7; col>0; --col)
                    for(int row=0; row<10; ++row)
                        leds[row*16+col] = getLed(row, col-1);
                for(int row=0; row<10; ++row)
                    leds[row] = 0;
                break;
            case 0x0: // shift right
                for(int col=0; col<7; ++col)
                    for(int row=0; row<10; ++row)
                        leds[row*16+col] = getLed(row, col+1);
                for(int row=0; row<10; ++row)
                    leds[row+7] = 0;
                break;
            case 0x2:
                for(int col=0; col<8; ++col)
                    for(int row=0; row<9; ++row)
                        leds[row*16+col] = getLed(row+1, col);
                for(int col=0; col<8; ++col)
                    leds[9+col] = 0;
                break;
            case 0x3:
                for(int col=0; col<8; ++col)
                    for(int row=9; row>0; --row)
                        leds[row*16+col] = getLed(row-1, col);
                for(int col=0; col<8; ++col)
                    leds[col] = 0;
                break;
            }
        }
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

    public int getColumn(){
        return getSensorValue(SensorType.X_SENSOR, 0, getWidth());
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

    public int getRow(){
        return getSensorValue(SensorType.X_SENSOR, 0, getHeight());
    }

    /**
     * Convenience method for getting Z sensor value
     */
    public float getZ(){
        return getSensorValue(SensorType.Z_SENSOR);
    }

    public float getZ(float min, float max){
        return getSensorValue(SensorType.Z_SENSOR, min, max);
    }

    public int getZ(int min, int max){
        return getSensorValue(SensorType.Z_SENSOR, min, max);
    }

//     public boolean isButtonPressed(int buttonIndex){
//         // todo!
//         // remove?
//         return false;
//     }

}