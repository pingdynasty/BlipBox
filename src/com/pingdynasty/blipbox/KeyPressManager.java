package com.pingdynasty.blipbox;

import org.apache.log4j.Logger;

public abstract class KeyPressManager implements KeyEventHandler {
    private static final Logger log = Logger.getLogger(KeyPressManager.class);

    private int xval, yval, tval;
    private int lastCol, lastRow;
    private int newCol, newRow;
    private boolean pressed;
    private int sensitivity = 200;
    private int columnCount = 10;
    private int rowCount = 8;

    public int getColumn(){
        return lastCol;
    }

    public int getRow(){
        return lastRow;
    }

    public boolean isPressed(){
        return pressed;
    }

    public int getNumberOfColumns(){
        return columnCount;
    }

    public void setNumberOfColumns(int columnCount){
        this.columnCount = columnCount;
    }

    public int getNumberOfRows(){
        return rowCount;
    }

    public void setNumberOfRows(int rowCount){
        this.rowCount = rowCount;
    }

    public void setSensitivity(int sensitivity){
        this.sensitivity = sensitivity;
    }

    // todo: let the event handler register sensors with data handler
    public void sensorChange(SensorDefinition sensor){
        boolean process = false;
        switch(sensor.type){
        case X_SENSOR:
            xval = sensor.value;
            newCol = sensor.scale(columnCount);
            break;
        case Y_SENSOR:
            yval = sensor.value;
            newRow = sensor.scale(rowCount);
            break;
        case TOUCH_SENSOR:
            tval = sensor.value;
            process = true;
            break;
        }
        if(process){

//         System.out.print("touch  "+tval);
//         System.out.print("\tx  "+xval);
//         System.out.print("\ty  "+yval);
//         System.out.println();

            if(tval < sensitivity){
                if(newRow >= rowCount)
                    newRow = rowCount - 1;
                if(newCol >= columnCount)
                    newCol = columnCount - 1;
                if(newCol >= 0 && newRow >= 0){ // && newCol < columnCount && newRow < rowCount){
                    if(pressed != true){
                        pressed = true;
                        keyDown(newCol, newRow);
                    }else if(lastCol != newCol || lastRow != newRow){
                        keyChange(lastCol, lastRow, newCol, newRow);
                    }
                    lastCol = newCol;
                    lastRow = newRow;
                }else{
                    log.error("Column or row out of bounds: "+newCol+"/"+newRow);
                }
            }else if(pressed){
                pressed = false;
                keyUp(lastCol, lastRow);
            }
        }
    }

    abstract public void keyDown(int col, int row);
    abstract public void keyUp(int col, int row);
    abstract public void keyChange(int oldCol, int oldRow, int newCol, int newRow);
}