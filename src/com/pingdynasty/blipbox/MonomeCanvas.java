package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;
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

public class MonomeCanvas extends JPanel {
    private static final Logger log = Logger.getLogger(MonomeCanvas.class);
    private MonomeEventHandler eventhandler;
    private BlipBox sender;

    public class ConfigurationPanel extends JPanel {

        public ConfigurationPanel(){
            JPanel panel = new JPanel(new MigLayout());
            add(panel);

            panel.add(new Label("Follow Mode"), "label");
            JComboBox box = new JComboBox(sender.getFollowModes());
            box.setSelectedItem("Cross");
            box.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JComboBox box = (JComboBox)e.getSource();
                        String name = (String)box.getSelectedItem();
                        sender.setFollowMode(name);
                    }
                });
            panel.add(box, "wrap");

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

            JButton button = new JButton("Clear");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        sender.clear();
                        eventhandler.clear(); // clear any set points
                    }
                });
            panel.add(button, "span, align r, wrap");
        }
    }

    public class DisplayEffectPanel extends JPanel {
        public DisplayEffectPanel(){
            JPanel panel = new JPanel(new MigLayout());
            add(panel);

            panel.add(new Label("Effect"), "label");
            JComboBox box = new JComboBox(sender.getDisplayEffects());
            box.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JComboBox box = (JComboBox)e.getSource();
                        String name = (String)box.getSelectedItem();
                        sender.sendDisplayEffect(name);
                    }
                });
            panel.add(box, "wrap");
        }
    }

    public MonomeCanvas(MonomeEventHandler eventhandler, BlipBox sender){
        this.sender = sender;
        this.eventhandler = eventhandler;

        JTabbedPane tabs = new JTabbedPane();
        
        JPanel panel = new ConfigurationPanel();
        tabs.addTab("Setup", panel);

        panel = new DisplayEffectPanel();
        tabs.addTab("Effect", panel);

        add(tabs);

    }
}