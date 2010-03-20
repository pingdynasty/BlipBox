package com.pingdynasty.blipbox;

import java.awt.Color;
import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.FontMetrics;
import java.awt.geom.Rectangle2D;

public class MultiTouchscreenCanvas extends TouchscreenCanvas {
    private int[] pos;
    
    private static final Color[] colours = new Color[]{ 
        Color.RED, Color.GREEN, Color.BLUE, Color.MAGENTA, 
        Color.GRAY, Color.BLACK, Color.ORANGE, Color.DARK_GRAY
    };

    public void update(Graphics g){
//         super.paint(g);

        for(int i=0; i<pos.length-1; i+=2){
            g.setColor(colours[i/2]);
            g.drawOval(pos[i], pos[i+1], 5, 5);
        }
    }

    public void update(int[] pos){
        this.pos = pos;
        repaint();
    }
}