package org.blipbox;

public abstract class AbstractBlipAction implements BlipAction, BlipBoxInput {
    private int x, y, z;
    private int dx, dy;
    private long tapmoment;
    private long taptime = 200; // max milliseconds between double taps
    private boolean pressed;

    public void sensorChange(BlipSensor sensor){
        switch(sensor.getMessageId()){
        case SensorConfiguration.TOUCH_SENSOR_MSG_ID:
            z = sensor.getValue();
            if(sensor.getValue() == 0){
                if(pressed){
                    pressed = false;
                    release(dx, dy);
                }
            }else{
                position(dx, dy);
                if(!pressed){
                    long now = System.currentTimeMillis();
                    pressed = true;
                    if(now - tapmoment <= taptime){
                        tapmoment = now;
                        taptap(x, y);
                    }else{
                        tapmoment = now;
                        tap(x, y);
                    }
                }else{
                    drag(x, y, dx, dy);
                }
            }
            break;            
        case SensorConfiguration.X_SENSOR_MSG_ID:
            if(!pressed)
                x = sensor.getValue();
            dx = sensor.getValue();
            break;            
        case SensorConfiguration.Y_SENSOR_MSG_ID:
            if(!pressed)
                y = sensor.getValue();
            dy = sensor.getValue();
            break;            
        }
    }
    public void parameterValue(int pid, int value){}
    public void tap(int x, int y){}
    public void taptap(int x, int y){}
    public void release(int x, int y){}
    public void drag(int x, int y, int dx, int dy){}
    public void position(int x, int y){}

    public void setTapTime(int taptime){
        this.taptime = (long)taptime;
    }
}
