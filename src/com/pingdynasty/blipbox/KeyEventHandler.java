package com.pingdynasty.blipbox;

public interface KeyEventHandler extends SensorEventHandler {

    public void keyDown(int col, int row);
    public void keyUp(int col, int row);
    public void keyChange(int oldCol, int oldRow, int newCol, int newRow);
}