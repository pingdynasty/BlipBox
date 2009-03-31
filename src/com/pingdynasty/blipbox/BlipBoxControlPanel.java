package com.pingdynasty.blipbox;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.apache.log4j.Logger;
import net.miginfocom.swing.MigLayout;

public class BlipBoxControlPanel extends JPanel {
    private static final Logger log = Logger.getLogger(BlipBoxControlPanel.class);
    private BlipBox sender;
    private BlipBoxDataHandler receiver;
    private KeyPressManager eventhandler;

    private Integer[] speeds = {9600, 19200, 38400, 57600, 115200};

    public BlipBoxControlPanel(BlipBoxApplication application){

        this.sender = application.getBlipBoxSender();
        this.receiver = application.getBlipBoxReceiver();
        if(receiver.getEventHandler() instanceof KeyPressManager)
            this.eventhandler = (KeyPressManager)receiver.getEventHandler();

        JPanel panel = new JPanel(new MigLayout());
        add(panel);

        panel.add(new Label("Follow Mode"), "label");
        JComboBox box = new JComboBox(sender.getFollowModes());
        box.setSelectedItem("None");
        box.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e) {
                    JComboBox box = (JComboBox)e.getSource();
                    String name = (String)box.getSelectedItem();
                    sender.setFollowMode(name);
                }
            });
        panel.add(box, "wrap");

        if(eventhandler != null){
            panel.add(new Label("Sensitivity"), "label");
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(200, 20, 1200, 20));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        Integer value = (Integer)spinner.getValue();
                        eventhandler.setSensitivity(value);
                        sender.setSensitivity(value);
                    }
                });
            panel.add(spinner, "wrap");
        }

        panel.add(new Label("Message Frequency"), "label");
        JSpinner spinner = new JSpinner(new SpinnerNumberModel(sender.getMessageFrequency(), 0, 200, 5));
        spinner.addChangeListener(new ChangeListener(){
                public void stateChanged(ChangeEvent event){
                    JSpinner spinner = (JSpinner)event.getSource();
                    Integer value = (Integer)spinner.getValue();
                    sender.setMessageFrequency(value);
                }
            });
        panel.add(spinner, "wrap");

        panel.add(new Label("Brightness"), "label");
        spinner = new JSpinner(new SpinnerNumberModel(0xff, 0, 0xff, 0xf));
        spinner.addChangeListener(new ChangeListener(){
                public void stateChanged(ChangeEvent event){
                    JSpinner spinner = (JSpinner)event.getSource();
                    Integer value = (Integer)spinner.getValue();
                    sender.setBrightness(value);
                }
            });
        panel.add(spinner, "wrap");

        panel.add(new Label("Serial Speed"), "label");
        box = new JComboBox(speeds);
        box.setSelectedItem(application.getSerialSpeed()); // d'oh, this is not set in application yet
        box.setSelectedItem(115200);
        box.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e) {
                    JComboBox box = (JComboBox)e.getSource();
                    Integer value = (Integer)box.getSelectedItem();
                    sender.setSerialSpeed(value);
                }
            });
        panel.add(box, "wrap");
        panel.add(new Label("Note: Changing serial speed requires a device reboot!"), "wrap");

        JButton button = new JButton("Flush");
        button.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    sender.flush();
                }
            });
        panel.add(button, "span, align r, wrap");

        button = new JButton("Clear");
        button.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    sender.clear();
                }
            });
        panel.add(button, "span, align r, wrap");

        button = new JButton("Reset");
        button.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    receiver.reset();
                }
            });
        panel.add(button, "span, align r, wrap");
    }

}
