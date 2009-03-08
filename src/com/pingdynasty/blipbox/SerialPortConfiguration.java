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

public class SerialPortConfiguration {
    private static final Logger log = Logger.getLogger(BlipBoxDataHandler.class);

    private BlipBoxDataHandler service;
    private Integer[] speeds = {9600, 19200, 38400, 57600, 115200};
    private JComboBox serialspeed;
    private JComboBox serialport;
    private List<String> ports;
    private JFrame frame;
    private Action updateAction; // action to perform when configuration is saved/updated
    private Action cancelAction; // action to perform when configuration is cancelled

    public SerialPortConfiguration(BlipBoxDataHandler service){
        this.service = service;
    }

    public void init(){
    }

    public JPanel getPanel(){

        // create serial port combo box
        List<String> ports = SerialDataHandler.getSerialPorts();
        serialport = new JComboBox(ports.toArray());
        if(ports.contains(service.getSerialPortName()))
            serialport.setSelectedItem(service.getSerialPortName());
        
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
                    if(ports.contains(service.getSerialPortName()))
                        serialport.setSelectedItem(service.getSerialPortName());
                }
            });

        JPanel content = new JPanel();
        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));

        JLabel label = new JLabel("Port");
        label.setLabelFor(serialport);
        JPanel panel = new JPanel();
        panel.add(label);
        panel.add(serialport);
        panel.add(button);
        content.add(panel);

        label = new JLabel("Speed");
        label.setLabelFor(serialspeed);
        panel = new JPanel();
        panel.add(label);
        panel.add(serialspeed);
        content.add(panel);

        return content;
    }

    public JFrame getFrame(){
        if(frame == null){
            JComponent panel = getPanel();
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
            panel.add(box);
            frame = new JFrame("Serial port configuration");
            frame.setContentPane(panel);
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