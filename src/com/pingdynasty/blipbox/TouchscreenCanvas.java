package com.pingdynasty.blipbox;

import java.awt.Color;
import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.FontMetrics;
import java.awt.geom.Rectangle2D;

public class TouchscreenCanvas extends Canvas {
    private int tValue, xValue, yValue;

    private double xMax = 1024;
    private double yMax = 1024;
    private double xMin = 0;
    private double yMin = 0;
    private double xInc = 1;

    private double xTicLen = (yMax-yMin)/50;
    private double yTicLen = (xMax-xMin)/50;
    private double xTicInt = 20.0;
    private double yTicInt = 20.0;

    private double xScale;
    private double yScale;
    
    private static final Color[] colours = new Color[]{ 
        Color.RED, Color.GREEN, Color.BLUE, Color.MAGENTA, 
        Color.GRAY, Color.BLACK, Color.ORANGE, Color.DARK_GRAY
    };

    public TouchscreenCanvas(){
    }

    void drawAxes(Graphics g){
        g.drawString("" + (int)xMin,
                     getTheX(xMin),
                     getTheY(xTicLen/2)-2);
        g.drawString("" + (int)yMin,
                     getTheX(yTicLen/2)+2,
                     getTheY(yMin));

        int xMaxInt = (int)xMax;
        String xMaxStr = "" + xMaxInt;
        char[] array = Integer.toString((int)xMax).toCharArray();
        FontMetrics fontMetrics = g.getFontMetrics();
        
        Rectangle2D r2d = fontMetrics.getStringBounds(array,0,array.length,g);
        int labWidth = (int)(r2d.getWidth());
        int labHeight = (int)(r2d.getHeight());
        g.drawString("" + (int)xMax,
                     getTheX(xMax)-labWidth,
                     getTheY(xTicLen/2)-2);
        g.drawString("" + (int)yMax,
                     getTheX(yTicLen/2)+2,
                     getTheY(yMax)+labHeight);
        g.drawLine(getTheX(xMin),
                   getTheY(0.0),
                   getTheX(xMax),
                   getTheY(0.0));

        g.drawLine(getTheX(0.0),
                   getTheY(yMin),
                   getTheX(0.0),
                   getTheY(yMax));
        xTics(g);
        yTics(g);
    }

    void xTics(Graphics g){
        double xDoub = 0;
        int x = 0;
        int topEnd = getTheY(xTicLen/2);
        int bottomEnd = getTheY(-xTicLen/2);
        if(topEnd < 5){
            topEnd = 5;
            bottomEnd = -5;
        }
        while(xDoub < xMax){
            x = getTheX(xDoub);
            g.drawLine(x,topEnd,x,bottomEnd);
            xDoub += xTicInt;
        }
        xDoub = 0;
        while(xDoub > xMin){
            x = getTheX(xDoub);
            g.drawLine(x,topEnd,x,bottomEnd);
            xDoub -= xTicInt;
        }
    }

    void yTics(Graphics g){
        double yDoub = 0;
        int y = 0;
        int rightEnd = getTheX(yTicLen/2);
        int leftEnd = getTheX(-yTicLen/2);
        while(yDoub < yMax){
            y = getTheY(yDoub);
            g.drawLine(rightEnd,y,leftEnd,y);
            yDoub += yTicInt;
        }
        yDoub = 0;
        while(yDoub > yMin){
            y = getTheY(yDoub);
            g.drawLine(rightEnd,y,leftEnd,y);
            yDoub -= yTicInt;
        }
    }

    int getTheY(double y){
        double yDoub = (yMax+yMin)-y;
        int yInt = (int)(yDoub*yScale);
        return yInt;
    }

    int getTheX(double x){
        return (int)(x*xScale);
    }

    public void paint(Graphics g){
        int height = getSize().height;
        int width = getSize().width;

        // Calculate the scale factors
        xScale = width/(xMax-xMin);
        yScale = height/(yMax-yMin);

        // Set the origin based on the
        // minimum values in x and y
        g.translate((int)((0-xMin)*xScale),
                    (int)((0-yMin)*yScale));

        g.setColor(Color.DARK_GRAY);
        drawAxes(g);

        int xpos = getTheX(xValue);
        int ypos = getTheY(yValue);

        g.drawString(xValue+","+yValue+","+tValue, xpos, ypos);
    }

    public void update(int tValue, int xValue, int yValue){
        this.tValue = tValue;
        this.xValue = xValue;
        this.yValue = yValue;
        repaint();
    }
}