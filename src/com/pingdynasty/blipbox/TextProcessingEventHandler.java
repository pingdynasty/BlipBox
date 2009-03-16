package com.pingdynasty.blipbox;

public class TextProcessingEventHandler extends KeyPressManager {
    private BlipBox service;

    public TextProcessingEventHandler(BlipBox service){
        this.service = service;
    }

//     public void sensorChange(SensorDefinition sensor){
//         super.sensorChange(sensor);
//     }

    public void keyDown(int col, int row){
    }

    public void keyUp(int col, int row){
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        if(oldCol < newCol)
            service.shiftRight(newCol - oldCol);
        else if(oldCol > newCol)
            service.shiftLeft(oldCol - newCol);
        if(oldRow < newRow)
            service.shiftUp(newRow - oldRow);
        else if(oldRow > newRow)
            service.shiftDown(oldRow - newRow);
    }

}