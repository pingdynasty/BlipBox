package org.blipbox;

public interface BlipAction {
    public void press(Position pos);
    public void drag(Position from, Position to);
    public void release(Position pos);
    public void tap(Position pos);
    public void taptap(Position pos);
    public void position(Position pos);
}
