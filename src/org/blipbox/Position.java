package org.blipbox;

public class Position {
    private BlipSensor x;
    private BlipSensor y;
    private BlipSensor z;

    public Position(){}

    public Position(BlipSensor x, BlipSensor y, BlipSensor z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public void setX(BlipSensor x){
        this.x = x;
    }

    public void setY(BlipSensor y){
        this.y = y;
    }

    public void setZ(BlipSensor z){
        this.z = z;
    }

    public int getColumn(){
        return x.scale(0, 10);
    }

    public int getRow(){
        return y.scale(0, 8);
    }

    public float getX(){
        return x.getValue();
    }

    public float getY(){
        return y.getValue();
    }

    public float getZ(){
        return z.getValue();
    }

    public float getX(float min, float max){
        return x.scale(min, max);
    }

    public float getY(float min, float max){
        return y.scale(min, max);
    }

    public float getZ(float min, float max){
        return z.scale(min, max);
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

    public int getRawX(){
        return x.getRawValue();
    }

    public int getRawY(){
        return y.getRawValue();
    }

    public int getRawZ(){
        return z.getRawValue();
    }

    public String toString(){
        return "pos["+x.getValue()+","+y.getValue()+","+z.getValue()+"]";
    }
}