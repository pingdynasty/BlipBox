package org.blipbox;

public abstract class AbstractBlipAction implements BlipBoxInput {
    private int x, y, z;
    private int dx, dy;
    private long tapmoment;
    private long taptime = 60; // max milliseconds between double taps
    private boolean pressed;

    public void sensorChange(BlipSensor sensor){
        switch(sensor.getMessageId()){
        case SensorConfiguration.TOUCH_SENSOR_MSG_ID:
            z = sensor.getValue();
            if(sensor.getValue() == 0){
                if(pressed){
                    pressed = false;
                    release(x, y);
                }
            }else{
                if(!pressed){
                    pressed = true;
                    tap(x, y);
                    long now = System.currentTimeMillis();
                    if(now - taptime <= tapmoment){
                        tapmoment = now;
                        taptap(x, y);
                    }else{
                        tapmoment = now;
                    }
                }else{
                    drag(x, y, dx, dy);
                }
            }
            break;            
        case SensorConfiguration.X_SENSOR_MSG_ID:
            if(pressed)
                dx = sensor.getValue();
            else
                x = sensor.getValue();
            break;            
        case SensorConfiguration.Y_SENSOR_MSG_ID:
            if(pressed)
                dy = sensor.getValue();
            else
                y = sensor.getValue();
            break;            
        }
    }
    public void parameterValue(int pid, int value){}
    public void tap(int x, int y){}
    public void taptap(int x, int y){}
    public void release(int x, int y){}
    public void drag(int initialX, int initialY, int newX, int newY){}
}
