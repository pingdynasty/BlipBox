package org.blipbox;

public interface BlipBoxOutput {
    public void setLed(int x, int y, int value);
    public void setLed(int index, int value);
    public void setLedColumn(int col, int data);
    public void setLedRow(int row, int data);
    public void clear(); // same as fill(0);
    public void fill(int value);
    public void writeText(String str, long delay);
    public void writeCharacter(int pos, char c);
    public void shiftHorizontal(int steps); // negative steps for left, positive for right
    public void shiftVertical(int steps); // negative steps for down, positive for up
    public void setParameter(int pid, int value);
    public void requestParameter(int pid);
}
