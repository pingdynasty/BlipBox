package com.pingdynasty.blipbox;

import java.io.OutputStream;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.apache.log4j.Logger;

public class BlipBoxApplication {
    private static final Logger log = Logger.getLogger(BlipBoxApplication.class);

    protected BlipBox sender;
    protected BlipBoxDataHandler receiver;
    protected SensorConfiguration configuration;
//     protected SensorEventHandler eventhandler;
    private SerialPortConfiguration serialconfig;
    private String serialport;
    private int serialspeed;

    public BlipBoxApplication(String config){
        configuration = SensorConfiguration.createSensorConfiguration(config);
        receiver = new BlipBoxDataHandler();
        receiver.setSensorConfiguration(configuration);
        serialconfig = new SerialPortConfiguration(this);
        serialconfig.init();
        sender = new BlipBox();
    }

    public int getSerialSpeed(){
        return serialspeed;
    }

    public void setSerialSpeed(int serialspeed){
        this.serialspeed = serialspeed;
    }

    public String getSerialPort(){
        return serialport;
    }

    public void setSerialPort(String port){
        this.serialport = port;
    }

    public void setLogStream(OutputStream logStream){
        receiver.setLogStream(logStream);
    }

    public JFrame createFrame(){
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 400);
        frame.setTitle("BlipBox");
        frame.setJMenuBar(createMenuBar());
        return frame;
    }

    public JMenuBar createMenuBar(){
        JMenuBar menubar = new JMenuBar();
        menubar.add(createSettingsMenu());
        return menubar;
    }

    public JMenu createSettingsMenu(){
        JMenu menu = new JMenu("Settings");
        JMenuItem item = new JMenuItem("Serial Port");
        item.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    try{
                        serialconfig.open();
                    }catch(Exception exc){log.error("Failed to open serial port configuration", exc);}
                }
            });
        menu.add(item);
        return menu;
    }

    public void openSerialPort() {
        receiver.openSerialPort(serialport, serialspeed);
        sender.setOutputStream(receiver.getOutputStream());
    }

    public void openSerialPort(String port, int speed) {
        this.serialport = port;
        this.serialspeed = speed;
        openSerialPort();
    }

    public void start(){
        // does nothing, forever
        for(;;);
    }
}
