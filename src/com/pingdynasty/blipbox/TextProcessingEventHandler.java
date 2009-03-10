package com.pingdynasty.blipbox;

public class TextProcessingEventHandler extends KeyPressManager {
    private BlipBoxDataHandler service;

    public TextProcessingEventHandler(BlipBoxDataHandler service){
        this.service = service;
    }

//     public void sensorChange(SensorDefinition sensor){
//         super.sensorChange(sensor);
//     }

    public void keyDown(int col, int row){
        System.out.println("key down:\t "+col+"\t  "+row);
    }

    public void keyUp(int col, int row){
        System.out.println("key up:\t "+col+"\t  "+row);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        System.out.println("key change:\t "+oldCol+"\t  "+oldRow+"\t  "+newCol+"\t  "+newRow);
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