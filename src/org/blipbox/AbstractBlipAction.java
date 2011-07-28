package org.blipbox;

public abstract class AbstractBlipAction implements BlipAction, BlipBoxInput {
    private Position apos = new Position();
    private Position bpos = new Position();
    private long lastpressed;
    private long lasttapped;
    private static final long TAP_THRESHOLD = 300; // max milliseconds between double taps
    private static final long TAPTAP_THRESHOLD = 600; // max milliseconds between double taps
    private boolean pressed;

    private int firstcol, firstrow;
    public void sensorChange(BlipSensor sensor){
        switch(sensor.getMessageId()){
        case SensorConfiguration.Z_SENSOR_MSG_ID:
	    long now = System.currentTimeMillis();
            bpos.setZ(sensor);
            if(sensor.getValue() == 0){
                if(pressed){
                    pressed = false;
		    if(now - lasttapped < TAPTAP_THRESHOLD &&
                       firstcol == bpos.getColumn() &&
                       firstrow == bpos.getRow()){
                        taptap(apos);
		    }else if(now - lastpressed < TAP_THRESHOLD &&
                             firstcol == bpos.getColumn() &&
                             firstrow == bpos.getRow()){
			lasttapped = now;
                        tap(apos);
		    }
                    release(bpos);
                }
            }else{
                if(!pressed){
                    if(firstcol != apos.getColumn() ||
                       firstrow != apos.getRow())
                        lasttapped = -1;
                    firstcol = apos.getColumn();
                    firstrow = apos.getRow();
                    apos.setZ(sensor);
                    pressed = true;
		    lastpressed = now;
		    press(apos);
                }else{
                    drag(apos, bpos);
                }
                position(bpos);
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

    public void parameterValue(Parameter param, int value){}
    public void press(Position pos){}
    public void drag(Position from, Position to){}
    public void release(Position pos){}
    public void tap(Position pos){}
    public void taptap(Position pos){}
    public void position(Position pos){}
}
