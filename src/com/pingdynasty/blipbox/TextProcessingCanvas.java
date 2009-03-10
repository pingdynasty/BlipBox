package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;
import javax.swing.JFrame;
import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.*;
import java.awt.event.*;
import javax.swing.JPanel;
import javax.swing.*;
import javax.swing.event.*;
import javax.sound.midi.*;
import org.apache.log4j.Logger;
import com.pingdynasty.midi.*;
import com.pingdynasty.midi.SpringUtilities;
import net.miginfocom.swing.MigLayout;

public class TextProcessingCanvas extends JPanel {
    private static final Logger log = Logger.getLogger(TextProcessingCanvas.class);
    private TextProcessingEventHandler eventhandler;
    private BlipBoxDataHandler service;

    public class ConfigurationPanel extends JPanel {

        public ConfigurationPanel(){
            JPanel panel = new JPanel(new MigLayout());
            add(panel);

            panel.add(new Label("Follow Mode"), "label");
            JComboBox box = new JComboBox(service.getFollowModes());
            box.setSelectedItem("Cross");
            box.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JComboBox box = (JComboBox)e.getSource();
                        String name = (String)box.getSelectedItem();
                        service.setFollowMode(name);
                    }
                });
            panel.add(box, "wrap");

            panel.add(new Label("Sensitivity"), "label");
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(200, 20, 600, 20));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        Integer value = (Integer)spinner.getValue();
                        eventhandler.setSensitivity(value);
                        service.setSensitivity(value);
                    }
                });
            panel.add(spinner, "wrap");

            JButton button = new JButton("Clear");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        service.clear();
                    }
                });
            panel.add(button, "span, align r, wrap");
        }
    }

    public class TextInputPanel extends JPanel {
        private int pos;
        public TextInputPanel(int position){
            this.pos = position;
            JPanel panel = new JPanel(new MigLayout());
            add(panel);
            panel.add(new Label("Write text"), "label");
            JTextField field = new JTextField(1);
            field.setEditable(true);
            field.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JTextField field = (JTextField)e.getSource();
                        String name = (String)field.getText();
                        service.sendCharacter(pos, name.charAt(0));
                    }
                });
            panel.add(field);
        }
    }

    public TextProcessingCanvas(TextProcessingEventHandler eventhandler, 
                                BlipBoxDataHandler service){
        this.eventhandler = eventhandler;
        this.service = service;

        JTabbedPane tabs = new JTabbedPane();
        
        JPanel panel = new ConfigurationPanel();
        tabs.addTab("Setup", panel);

        panel = new TextInputPanel(0);
        tabs.addTab("Text Left", panel);

        panel = new TextInputPanel(5);
        tabs.addTab("Text Right", panel);

        add(tabs);

    }
}