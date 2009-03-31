package com.pingdynasty.blipbox;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.apache.log4j.Logger;
import net.miginfocom.swing.MigLayout;

public class TextProcessingCanvas extends JPanel {
    private static final Logger log = Logger.getLogger(TextProcessingCanvas.class);
    private TextProcessingEventHandler eventhandler;
    private BlipBox sender;

    public class SetLedPanel extends JPanel {
        private int col = 1;
        private int row = 1;
        private int brightness = 0xff;
        public SetLedPanel(){
            JPanel panel = new JPanel(new MigLayout());
            add(panel);
            
            panel.add(new Label("Column"), "label");
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(col, 1, 10, 1));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        col = (Integer)spinner.getValue();
                    }
                });
            panel.add(spinner, "wrap");

            panel.add(new Label("Row"), "label");
            spinner = new JSpinner(new SpinnerNumberModel(row, 1, 8, 1));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        row = (Integer)spinner.getValue();
                    }
                });
            panel.add(spinner, "wrap");

            spinner = new JSpinner(new SpinnerNumberModel(brightness, 0, 255, 1));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        brightness = (Integer)spinner.getValue();
                    }
                });
            panel.add(spinner, "wrap");

            JButton button = new JButton("Set");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        sender.setLed(col-1, row-1, brightness);
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

    public class CharacterInputPanel extends JPanel {
        private int col;
        private char character = '\0';
        public CharacterInputPanel(int position){
            this.col = position;
            JPanel panel = new JPanel(new MigLayout());
            add(panel);

            panel.add(new Label("Column"), "label");
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(col, 1, 10, 1));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        col = (Integer)spinner.getValue();
                    }
                });
            panel.add(spinner, "wrap");

            panel.add(new Label("Input character"), "label");
            JTextField field = new JTextField(1);
            field.setEditable(true);
            field.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JTextField field = (JTextField)e.getSource();
                        String str = (String)field.getText();
                        character = str.charAt(0);
                    }
                });
            panel.add(field, "wrap");

            JButton button = new JButton("Write");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        sender.sendCharacter(col-1, character);
                    }
                });
            panel.add(button, "span, align r, wrap");
        }
    }

    public class StringInputPanel extends JPanel {
        private int delay = 200;
        private String text = "";

        public StringInputPanel(){
            JPanel panel = new JPanel(new MigLayout());
            add(panel);
            panel.add(new Label("Write text"), "label");
            JTextField field = new JTextField(20);
            field.setEditable(true);
            field.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JTextField field = (JTextField)e.getSource();
                        text = (String)field.getText();
                        sender.sendString(text, delay);
                    }
                });
            panel.add(field);

            panel.add(new Label("Speed"), "label");
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(delay, 10, 800, 20));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        delay = (Integer)spinner.getValue();
                    }
                });
            panel.add(spinner, "wrap");

//             JButton button = new JButton("Write");
//             button.addActionListener(new AbstractAction(){
//                     public void actionPerformed(ActionEvent e){
//                         sender.sendString(text, delay);
//                     }
//                 });
//             panel.add(button, "span, align r, wrap");
        }
    }

    public TextProcessingCanvas(BlipBoxApplication application,
                                TextProcessingEventHandler eventhandler, 
                                BlipBox sender){
        this.eventhandler = eventhandler;
        this.sender = sender;

        JTabbedPane tabs = new JTabbedPane();
        
        JPanel panel = new BlipBoxControlPanel(application);
        tabs.addTab("Control", panel);

        panel = new DisplayEffectPanel();
        tabs.addTab("Effect", panel);

        panel = new CharacterInputPanel(1);
        tabs.addTab("Character", panel);

        panel = new StringInputPanel();
        tabs.addTab("Text", panel);

        panel = new SetLedPanel();
        tabs.addTab("Set Led", panel);


        add(tabs);

    }
}