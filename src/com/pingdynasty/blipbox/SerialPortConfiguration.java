package com.pingdynasty.blipbox;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import com.pingdynasty.midi.SpringUtilities;
import org.apache.log4j.Logger;
import net.miginfocom.swing.MigLayout;

public class SerialPortConfiguration {
    private static final Logger log = Logger.getLogger(BlipBoxDataHandler.class);

    private BlipBoxApplication service;
    private Integer[] speeds = {9600, 19200, 38400, 57600, 115200};
    private JComboBox serialspeed;
    private JComboBox serialport;
    private List<String> ports;
    private JFrame frame;
    private Action updateAction; // action to perform when configuration is saved/updated
    private Action cancelAction; // action to perform when configuration is cancelled

    public SerialPortConfiguration(BlipBoxApplication service){
        this.service = service;
    }

    public void init(){
    }

    public JPanel getPanel(){

        JPanel content = new JPanel();
        content.setLayout(new MigLayout());

        // create serial port combo box
        List<String> ports = SerialDataHandler.getSerialPorts();
        serialport = new JComboBox(ports.toArray());
        if(ports.contains(service.getSerialPort()))
            serialport.setSelectedItem(service.getSerialPort());
        
        // create serial speed combo box
        serialspeed = new JComboBox(speeds);
        serialspeed.setSelectedItem(service.getSerialSpeed());

        JButton button = new JButton("Refresh");
        button.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent event){
                    serialport.removeAllItems();
                    List<String> ports = SerialDataHandler.getSerialPorts();
                    for(String port : ports)
                        serialport.addItem(port);
                    if(ports.contains(service.getSerialPort()))
                        serialport.setSelectedItem(service.getSerialPort());
                }
            });

        JLabel label = new JLabel("Port");
        label.setLabelFor(serialport);
        content.add(label, "label");
        content.add(serialport);
        content.add(button, "wrap");

        label = new JLabel("Speed");
        label.setLabelFor(serialspeed);
        content.add(label, "label");
        content.add(serialspeed);

        return content;
    }

    public JFrame getFrame(){
        if(frame == null){
            Box box = Box.createHorizontalBox();
            JButton button = new JButton("update");
            if(updateAction != null)
                button.addActionListener(updateAction);
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent event){
                        frame.setVisible(false);
                        String port = (String)serialport.getSelectedItem();
                        Integer speed = (Integer)serialspeed.getSelectedItem();
                        try{
                            service.openSerialPort(port, speed);
                        }catch(Exception exc){
                            log.error("Failed to open serial port "+port, exc);
                        }
                    }
                });
            box.add(button);
            button = new JButton("cancel");
            if(cancelAction != null)
                button.addActionListener(cancelAction);
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent event){
                        frame.setVisible(false);
                    }
                });
            box.add(button);

            JPanel content = new JPanel(new MigLayout());
            content.add(getPanel(), "wrap");
            content.add(box, "align center");
            frame = new JFrame("Serial port configuration");
            frame.setContentPane(content);
            frame.pack();
        }
        return frame;
    }

    public void open(){
        if(frame == null)
            getFrame();
        frame.setVisible(true);
    }

    public void close(){
        frame.setVisible(false);
    }
}