package com.pingdynasty.blipbox;

public class MonomeEventHandler extends KeyPressManager {
    private MonomeOutput monome;

    private boolean[][] keystates = new boolean[16][16];

    public MonomeEventHandler(MonomeOutput monome){
        this.monome = monome;
    }

//     public void sensorChange(SensorDefinition sensor){
//         super.sensorChange(sensor);
//     }

    public boolean toggle(int col, int row){
        keystates[col][row] = !keystates[col][row];
        return keystates[col][row];
    }

    public void keyDown(int col, int row){
        monome.press(col, row, toggle(col, row));
    }

    public void keyUp(int col, int row){
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        monome.press(oldCol, oldRow, toggle(oldCol, oldRow));
        monome.press(newCol, newRow, toggle(newCol, newRow));
    }

    public void clear(){
        for(int x=0; x<keystates.length; ++x){
            for(int y=0; y<keystates[0].length; ++y){
                if(keystates[x][y])
                    monome.press(x, y, toggle(x, y));
//                 keystates[x][y] = false;
//                 monome.press(x, y, false);
            }
        }
    }
}