package com.pingdynasty.blipbox;

public class MonomeEventHandler extends KeyPressManager {
    protected MonomeOutput monome;

    public MonomeEventHandler(MonomeOutput monome){
        this.monome = monome;
    }

//     public void sensorChange(SensorDefinition sensor){
//         super.sensorChange(sensor);
//     }

    public void keyDown(int col, int row){
        monome.press(col, row, true);
    }

    public void keyUp(int col, int row){
        monome.press(col, row, false);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        monome.press(oldCol, oldRow, false);
        monome.press(newCol, newRow, true);
    }

    public void clear(){
    }

}