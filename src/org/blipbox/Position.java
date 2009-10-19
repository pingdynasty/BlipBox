package org.blipbox;

public class Position {
    private BlipSensor x;
    private BlipSensor y;
    private BlipSensor z;

    public Position(BlipSensor x, BlipSensor y, BlipSensor z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public int getX(){
        return x.getValue();
    }

    public int getY(){
        return y.getValue();
    }

    public int getZ(){
        return z.getValue();
    }

    public int getX(int min, int max){
        return x.scale(min, max);
    }

    public int getY(int min, int max){
        return y.scale(min, max);
    }

    public int getZ(int min, int max){
        return z.scale(min, max);
    }

}