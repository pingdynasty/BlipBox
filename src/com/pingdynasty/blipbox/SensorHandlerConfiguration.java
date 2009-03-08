package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;

public class SensorHandlerConfiguration implements KeyEventHandler {
    private KeyEventHandler keyhandler;
    private boolean pitchbend, aftertouch;
    private Map<SensorType, SensorEventHandler> parameters = new HashMap<SensorType, SensorEventHandler>();

    public void registerSensorEventHandler(SensorType type, SensorEventHandler handler){
        parameters.put(type, handler);
    }

    public void registerKeyEventHandler(KeyEventHandler keyhandler){
        this.keyhandler = keyhandler;
//         parameters.put(Sensors.Y_SENSOR, keyhandler);
    }

    public void sensorChange(SensorDefinition sensor){
        SensorEventHandler handler = parameters.get(sensor.type);
        if(handler != null)
            handler.sensorChange(sensor);
    }

    public void keyDown(int col, int row){
        if(keyhandler != null)
            keyhandler.keyDown(col, row);
    }

    public void keyUp(int col, int row){
        if(keyhandler != null)
            keyhandler.keyUp(col, row);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        if(keyhandler != null)
            keyhandler.keyChange(oldCol, oldRow, newCol, newRow);
    }
}
