package com.pingdynasty.blipbox;

public interface MonomeInput {

    public String getPrefix();

    public void led(int x, int y, boolean state);
    public void led_col(int col, int data);
    public void led_row(int row, int data);

    public void clear(boolean state);
    public void test(boolean state);
//     public void test(int unit, boolean state);
//     public void frame(int[] data); // data is an int[8]

    public void prefix(String prefix);
//     public void prefix(int unit, String prefix);

//     public void cable(int orientation);
//     public void offset(int x, int y);
//     public void intensity(int intensity);
//     public void cable(int unit, int orientation);
//     public void offset(int unit, int x, int y);
//     public void intensity(int intensity);
//     public void report();

}