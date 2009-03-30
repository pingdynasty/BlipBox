package com.pingdynasty.blipbox;

import javax.swing.JFrame;
import java.awt.Canvas;

public class GraphingEventHandler extends LoggingEventHandler {

    private TouchscreenCanvas canvas;
    private int xval, yval, tval;

    public GraphingEventHandler(){
        canvas = new TouchscreenCanvas();
    }

    public Canvas getCanvas(){
        return canvas;
    }

    public void sensorChange(SensorDefinition sensor){
        switch(sensor.type){
        case X_SENSOR :
            xval = sensor.value;
            break;
        case Y_SENSOR :
            yval = sensor.value;
            break;
        case TOUCH_SENSOR :
            tval = sensor.value;
            break;
//         default:
//             System.out.println("unknown sensor type: "+type);
        }
        canvas.update(tval, xval, yval);
        super.sensorChange(sensor);
    }

}