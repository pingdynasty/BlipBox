package org.blipbox;

public interface BlipAction {
    public void tap(Position pos);
    public void taptap(Position pos);
    public void release(Position pos);
    public void drag(Position from, Position to);
    public void position(Position pos);
}
