package com.pingdynasty.blipbox;

import javax.swing.JFrame;
import java.awt.Canvas;

public class GraphingEventHandler implements BlipBoxEventHandler {

    private TouchscreenCanvas canvas;
    private int xval, yval, tval;

    public GraphingEventHandler(){
        canvas = new TouchscreenCanvas();
        JFrame frame = new JFrame();
        frame.add(canvas);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(860, 1024);
        frame.setVisible(true);

    }

    public void sensorChange(SensorType type, int value){
        switch(type){
        case X_SENSOR :
            xval = value;
            break;
        case Y_SENSOR :
            yval = value;
            break;
        case T_SENSOR :
            tval = value;
            break;
        default:
            System.out.println("unknown sensor type: "+type);
        }
        canvas.update(tval, xval, yval);
    }

    public void keyDown(byte col, byte row){
        System.out.println("key down:\t "+col+"\t  "+row);
    }

    public void keyUp(byte col, byte row){
        System.out.println("key up:\t "+col+"\t  "+row);
    }

    public void keyChange(byte oldCol, byte oldRow, byte newCol, byte newRow){
        System.out.println("key change:\t "+oldCol+"\t  "+oldRow+"\t  "+newCol+"\t  "+newRow);
    }

}