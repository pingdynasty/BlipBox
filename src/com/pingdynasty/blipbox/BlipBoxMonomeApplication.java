package com.pingdynasty.blipbox;

import org.apache.log4j.Logger;
import javax.swing.*;
import se.antimon.osc.OscCommunication;
import se.antimon.osc.OscReceiver;
import java.net.SocketException;
import java.net.UnknownHostException;

public class BlipBoxMonomeApplication 
    extends BlipBoxApplication 
    implements MonomeInput, MonomeOutput {

    private static final Logger log = Logger.getLogger(BlipBoxMonomeApplication.class);
    private MonomeEventHandler eventhandler;
    private OscReceiver oscreceiver;
    private OscCommunication osc = null;

    private int maxColumns = 9;
    private int maxRows = 7;

    private int hostPort = 8000;
    private int listenPort = 8080;

    private String prefix = "/test";

    private static final boolean DEBUG = false;

    public BlipBoxMonomeApplication(String config)
        throws SocketException, UnknownHostException {
        super(config);

        eventhandler = new MonomeEventHandler(this);
        receiver.setSensorEventHandler(eventhandler);

        log.info("Starting OSC communication on port: "+hostPort);
        osc = new OscCommunication(hostPort);
        log.info("Listening for OSC communication on port: "+listenPort);
        OscReceiver oscReceive = new MonomeOscReceiver(this);
        oscReceive.listen(listenPort);
//         clear();

        MonomeCanvas canvas = new MonomeCanvas(this, eventhandler, sender);
        JFrame frame = createFrame();
        frame.add(canvas);
        frame.setVisible(true);
    }

    private int brightness = 0xff;

    public void led(int x, int y, boolean state){
        sender.setLed(x, y, state ? brightness : 0);
    }

    public void led_col(int col, int data){
        for(int i=0; i<8; ++i)
            led(col, i, (data & (1 << i)) != 0);
    }

    public void led_row(int row, int data){
        for(int i=0; i<8; ++i)
            led(i, row, (data & (1 << i)) != 0);
    }

    public void frame(int[] data){
        if(data.length != 8)
            throw new IllegalArgumentException("frame expects exactly 8 bytes of data");
        for(int i=0; i<8; ++i)
            led_col(i, data[i]);
    }

    public void clear(boolean state){
        if(state)
            sender.clear();
        else
            flush();
    }

    protected void flush(){
        for(int x=0; x<8; ++x)
            for(int y=0; y<8; ++y)
                sender.setLed(x, y, brightness);
    }

    public void test(boolean state){
        clear(!state);
    }

//     public void test(int unit, boolean state);
//     public void frame(int[] data); // data is an int[8]

    public void prefix(String prefix){
        setPrefix(prefix);
// when prefix changes successfully (valid pattern), change is reported by sending out a report message: 
    }

    public void press(int x, int y, boolean state){
        osc.sendOscCommand(prefix + "/press", new Object[]{x, maxRows - y, state ? 1 : 0}, DEBUG);
    }

//     private void clear() {
//         osc.sendOscCommand(PREFIX + "/clear", null, DEBUG);
//     }

    public String getPrefix(){
        return prefix;
    }

    public void setPrefix(String prefix){
        this.prefix = prefix;
    }

    public void start(){
        sender.setFollowMode("None");
        log.info("Blipbox Monome OSC application started");
    }
}
