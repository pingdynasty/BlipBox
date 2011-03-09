package org.blipbox;

public abstract class AbstractBlipAction implements BlipAction, BlipBoxInput {
    private Position apos = new Position();
    private Position bpos = new Position();
    private long tapmoment;
    private long taptime = 200; // max milliseconds between double taps
    private boolean pressed;

    public void sensorChange(BlipSensor sensor){
        switch(sensor.getMessageId()){
        case SensorConfiguration.Z_SENSOR_MSG_ID:
            bpos.setZ(sensor);
            if(sensor.getValue() == 0){
                if(pressed){
                    pressed = false;
                    release(bpos);
                }
            }else{
                position(bpos);
                if(!pressed){
                    apos.setZ(sensor);
                    long now = System.currentTimeMillis();
                    pressed = true;
                    if(now - tapmoment <= taptime){
                        tapmoment = now;
                        taptap(apos);
                    }else{
                        tapmoment = now;
                        tap(apos);
                    }
                }else{
                    drag(apos, bpos);
                }
            }
            break;            
        case SensorConfiguration.X_SENSOR_MSG_ID:
            if(!pressed)
                apos.setX(sensor);
            bpos.setX(sensor);
            break;            
        case SensorConfiguration.Y_SENSOR_MSG_ID:
            if(!pressed)
                apos.setY(sensor);
            bpos.setY(sensor);
            break;            
        }
    }

    public void setTapTime(int taptime){
        this.taptime = (long)taptime;
    }

    public void parameterValue(Parameter param, int value){}
    public void tap(Position pos){}
    public void taptap(Position pos){}
    public void release(Position pos){}
    public void drag(Position from, Position to){}
    public void position(Position pos){}
}
