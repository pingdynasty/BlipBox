package org.blipbox;

public interface BlipAction {
    public void tap(int x, int y);
    public void taptap(int x, int y);
    public void release(int x, int y);
    public void drag(int x, int y, int dx, int dy);
    public void position(int x, int y);
}
