package com.pingdynasty.blipbox;

public class LoggingEventHandler extends KeyPressManager {

    private long millis = System.currentTimeMillis();

    public void sensorChange(SensorDefinition sensor){
        long now = System.currentTimeMillis();
        System.out.println("sensor "+sensor.type+":\t "+sensor.value+" (+"+(now - millis)+"ms)");
        millis = now;
        super.sensorChange(sensor);
    }

    public void keyDown(int col, int row){
        System.out.println("key down:\t "+col+"\t  "+row);
    }

    public void keyUp(int col, int row){
        System.out.println("key up:\t "+col+"\t  "+row);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        System.out.println("key change:\t "+oldCol+"\t  "+oldRow+"\t  "+newCol+"\t  "+newRow);
    }
}