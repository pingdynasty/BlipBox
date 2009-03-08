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

public class MidiConfigurationCanvas extends JPanel {
    private static final Logger log = Logger.getLogger(MidiOutputEventHandler.class);
    private MidiOutputEventHandler blipbox;
    private BlipBoxDataHandler service;

    private static final byte Y_NOTES_CC = 18;
    private static final byte X_CC = 20;
    private static final byte Y_CC = 21;
    private static final byte T_CC = 19;
    private static final byte POT_CC = 1;
    private static final byte BUTTON_CC = 22;

    private boolean enableSetupMode = false;
    private SerialPortConfiguration serialconfig;
    private BlipBoxMidiConfiguration midiconfig;

    private static final String[] controlTypes = 
    { "Unassigned", "Control Change", "Pitch Bend", "Basenote", "Scale" };

    public class BasicConfigurationPanel extends JPanel {
        private JComboBox scale;
        private JSpinner basenote;
        private JSpinner range;

        public BasicConfigurationPanel(){
//             setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
//             add(Box.createHorizontalGlue());
//             add(Box.createVerticalGlue());
            setLayout(new SpringLayout());

            JPanel panel = new JPanel();
            panel.add(new Label("Scale"));
            scale = new JComboBox(blipbox.getScaleNames());
            scale.setSelectedItem(blipbox.getCurrentScale());
            scale.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        String scalename = (String)scale.getSelectedItem();
                        blipbox.setScale(scalename);
                    }
                });
            panel.add(scale);
            add(panel);

            panel = new JPanel();
            panel.add(new Label("Basenote"));
            basenote = new JSpinner(new SpinnerNumberModel(1, 1, 127, 1));
            basenote.setValue(blipbox.getBasenote());
            basenote.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent e){
                        Integer value = (Integer)basenote.getValue();
                        blipbox.setBasenote(value);
                    }
                });
            panel.add(basenote);
            add(panel);

            panel = new JPanel();
            panel.add(new Label("Note Range"));
            range = new JSpinner(new SpinnerNumberModel(1, 1, 127, 1));
            range.setValue(10);
            range.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent e){
                        Integer value = (Integer)range.getValue();
                        blipbox.setNumberOfColumns(value);
                    }
                });
            panel.add(range);
            add(panel);

            panel = new JPanel();
            panel.add(new Label("Sensitivity"));
            JSpinner spinner = new JSpinner(new SpinnerNumberModel(200, 20, 600, 20));
            spinner.addChangeListener(new ChangeListener(){
                    public void stateChanged(ChangeEvent event){
                        JSpinner spinner = (JSpinner)event.getSource();
                        Integer value = (Integer)spinner.getValue();
                        blipbox.setSensitivity(value);
                        service.setSensitivity(value);
                    }
                });
            panel.add(spinner);
            add(panel);

            panel = new JPanel();
            panel.add(new Label("Follow Mode"));
            JComboBox box = new JComboBox(service.getFollowModes());
            box.setSelectedItem("Cross");
            box.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e) {
                        JComboBox box = (JComboBox)e.getSource();
                        String name = (String)box.getSelectedItem();
                        service.setFollowMode(name);
                    }
                });
            panel.add(box);
            add(panel);

            JButton button = new JButton("Configure MIDI");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        try{
                            midiconfig.open();
                        }catch(Exception exc){log.error("Failed to open MIDI configuration", exc);}
                    }
                });
            add(button);

            button = new JButton("Configure serial port");
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent e){
                        try{
                            serialconfig.open();
                        }catch(Exception exc){log.error("Failed to open serial configuration", exc);}
                    }
                });
            add(button);

            add(Box.createVerticalGlue());

            SpringUtilities.makeCompactGrid(this, 7, 1, 
                                            15, 15, 5, 5);
        }
    }

    public void updateMidiDevices(){
        try{
            blipbox.setMidiPlayer(midiconfig.getMidiOutput());
            blipbox.setChannel(midiconfig.getChannel());
        }catch(Exception exc){log.error(exc);}
    }

    public class ModeConfigurationPanel extends JPanel {
        private OperationMode mode;
        private NotePlayerConfigurationPanel notes;
        private CombinedParameterConfigurationPanel parameters;

        public ModeConfigurationPanel(OperationMode mode){
            this.mode = mode;
            notes = new NotePlayerConfigurationPanel();
            parameters = new CombinedParameterConfigurationPanel();
            setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
            add(notes);
            add(Box.createRigidArea(new Dimension(10,0)));
            add(parameters);
        }

        public OperationMode getOperationMode(){
            return mode;
        }

//         public void setup(SensorType sensor, String type){
//             parameters.setup(sensor, type, 0, 0, 0, 0);
//         }

        public void setup(SensorType sensor, String type, int channel, int cc, int min, int max){
            parameters.setup(sensor, type, channel, cc, min, max);
        }

        public void setup(boolean doPlay, boolean doPb, boolean doAt){
            notes.setup(doPlay, doPb, doAt);
        }

        public class ParameterConfigurationPanel extends JPanel {
            private String name;
            private SensorType type;
            private JComboBox typeList;
            private JSpinner channel;
            private JSpinner cc;
            private JSpinner min;
            private JSpinner max;

            public ParameterConfigurationPanel(String name, SensorType type){
                this.type = type;
                this.name = name;
                setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

                add(new Label(name));
                typeList = new JComboBox(controlTypes);
                typeList.addActionListener(new AbstractAction(){
                        public void actionPerformed(ActionEvent e) {
                            setType((String)typeList.getSelectedItem());
                            configure();
                        }
                    });
                add(typeList);

                add(new Label("channel"));
                channel = new JSpinner(new SpinnerNumberModel(1, 1, 16, 1));
                channel.setValue(1);
                channel.addChangeListener(new ChangeListener(){
                        public void stateChanged(ChangeEvent e){
                            configure();
                        }
                    });
                add(channel);

                add(new Label("cc"));
                cc = new JSpinner(new SpinnerNumberModel(21, 1, 127, 1));
                cc.setValue(21);
                cc.addChangeListener(new ChangeListener(){
                        public void stateChanged(ChangeEvent e){
                            configure();
                        }
                    });
                add(cc);

                add(new Label("min"));
                min = new JSpinner();
                min.addChangeListener(new ChangeListener(){
                        public void stateChanged(ChangeEvent e){
                            configure();
                        }
                    });
                add(min);

                add(new Label("max"));
                max = new JSpinner();
                max.setValue(127);
                max.addChangeListener(new ChangeListener(){
                        public void stateChanged(ChangeEvent e){
                            configure();
                        }
                    });
                add(max);

                JButton button = new JButton("invert");
                button.addActionListener(new AbstractAction(){
                        public void actionPerformed(ActionEvent e) {
                            int saved = (Integer)min.getValue();
                            min.setValue(max.getValue());
                            max.setValue(saved);
                        }
                    });
                add(button);
            }

            public void setup(String type, int channel, int cc, int min, int max){
//                 setType(type);
                typeList.setSelectedItem(type);
                this.channel.setValue(channel);
                this.cc.setValue(cc);
                this.min.setValue(min);
                this.max.setValue(max);
            }

            public void configure(){
                String type = (String)typeList.getSelectedItem();
                if("Control Change".equals(type)){
                    blipbox.configureControlChange(getOperationMode(), this.type, 
                                                   (Integer)channel.getValue(), (Integer)cc.getValue(), 
                                                   (Integer)min.getValue(), (Integer)max.getValue());
                }else if("Pitch Bend".equals(type)){
                    blipbox.configurePitchBend(getOperationMode(), this.type, 
                                               (Integer)channel.getValue(), 
                                               (Integer)min.getValue(), (Integer)max.getValue());
                }else if("Basenote".equals(type)){
                    blipbox.configureBaseNoteChange(getOperationMode(), this.type, 
                                                    (Integer)min.getValue(), (Integer)max.getValue());
                }else if("Scale".equals(type)){
                    blipbox.configureScaleChange(getOperationMode(), this.type);
                }else if("Unassigned".equals(type)){
                    blipbox.configureUnassigned(getOperationMode(), this.type);
                }
            }

            public void setType(String type){
                if("Control Change".equals(type)){
                    channel.setEnabled(true);
                    cc.setEnabled(true);
                    min.setEnabled(true);
                    max.setEnabled(true);
                    min.setValue(0);
                    max.setValue(127);
                }else if("Pitch Bend".equals(type)){
                    channel.setEnabled(true);
                    cc.setEnabled(false);
                    min.setEnabled(true);
                    max.setEnabled(true);
                    min.setValue(-8192);
                    max.setValue(8191);
                }else if("Basenote".equals(type)){
                    channel.setEnabled(false);
                    cc.setEnabled(false);
                    min.setEnabled(true);
                    max.setEnabled(true);
                    min.setValue(0);
                    max.setValue(127);
                }else if("Scale".equals(type)){
                    channel.setEnabled(false);
                    cc.setEnabled(false);
                    min.setEnabled(false);
                    max.setEnabled(false);
                }else if("Unassigned".equals(type)){
                    channel.setEnabled(false);
                    cc.setEnabled(false);
                    min.setEnabled(false);
                    max.setEnabled(false);
                }
            }
        }

        public class NotePlayerConfigurationPanel extends JPanel {
            private JCheckBox play;
            private JCheckBox pb;
            private JCheckBox at;
            private boolean doPlay = true;
            private boolean doAt, doPb;

            public NotePlayerConfigurationPanel(){
                setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
                play = new JCheckBox("Play notes", true);
                play.addActionListener(new AbstractAction(){
                        public void actionPerformed(ActionEvent e) {
                            doPlay = !doPlay;
                            if(doPlay){
                                pb.setEnabled(true);
                                at.setEnabled(true);
                            }else{
                                pb.setEnabled(false);
                                at.setEnabled(false);
                            }
                            configure();
                        }
                    });
                add(play);
                pb = new JCheckBox("Pitch Bend", false);
                pb.addActionListener(new AbstractAction(){
                        public void actionPerformed(ActionEvent e) {
                            doPb = !doPb;
                            configure();
                        }
                    });
                add(pb);
                at = new JCheckBox("Aftertouch", false);
                at.addActionListener(new AbstractAction(){
                        public void actionPerformed(ActionEvent e) {
                            doAt = !doAt;
                            configure();
                        }
                    });
                add(at);
                add(Box.createVerticalGlue());
            }

            public void configure(){
                blipbox.configureNotePlayer(getOperationMode(), doPlay, doPb, doAt);
            }

            public void setup(boolean doPlay, boolean doPb, boolean doAt){
                play.setSelected(doPlay);
                this.doPlay = doPlay;
                pb.setSelected(doPb);
                this.doPb = doPb;
                at.setSelected(doAt);
                this.doAt = doAt;
                configure();
            }

        }

        public class CombinedParameterConfigurationPanel extends JPanel {
            Map<SensorType, ParameterConfigurationPanel> panels = new HashMap<SensorType, ParameterConfigurationPanel>();

            public CombinedParameterConfigurationPanel(){
                setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
                ParameterConfigurationPanel panel = new ParameterConfigurationPanel("X parameter", SensorType.X_SENSOR);
                panels.put(SensorType.X_SENSOR, panel);
                add(panel);
                add(Box.createRigidArea(new Dimension(10,0)));
                panel = new ParameterConfigurationPanel("Y parameter", SensorType.Y_SENSOR);
                panels.put(SensorType.Y_SENSOR, panel);
                add(panel);
                add(Box.createRigidArea(new Dimension(10,0)));
                panel = new ParameterConfigurationPanel("Knob", SensorType.POT_SENSOR);
                panels.put(SensorType.POT_SENSOR, panel);
                add(panel);
                add(Box.createRigidArea(new Dimension(10,0)));
            }

            public void setup(SensorType sensor, String type, int channel, int cc, int min, int max){
                ParameterConfigurationPanel panel = panels.get(sensor);
                if(panel == null)
                    throw new RuntimeException("No configuration available for sensor type "+sensor);
                panel.setup(type, channel, cc, min, max);
            }
        }
    }

    private Map<OperationMode, ModeConfigurationPanel> modes = new HashMap<OperationMode, ModeConfigurationPanel>();

    public MidiConfigurationCanvas(MidiOutputEventHandler blipbox, BlipBoxDataHandler service){
        this.blipbox = blipbox;
        this.service = service;

        // setup midi configuration
        midiconfig = new BlipBoxMidiConfiguration();
        midiconfig.setUpdateAction(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    updateMidiDevices();
                }
            });
        serialconfig = new SerialPortConfiguration(service);
        try{
            // run configuration initialisation - set default serial port, midi config
            serialconfig.init();
            midiconfig.init();
            updateMidiDevices();
        }catch(Exception exc){
            log.error(exc, exc);
        }

        JTabbedPane tabs = new JTabbedPane();
        
        BasicConfigurationPanel basic = new BasicConfigurationPanel();
        tabs.addTab("Setup", basic);

        ModeConfigurationPanel panel = new ModeConfigurationPanel(OperationMode.CROSS);
        modes.put(panel.getOperationMode(), panel);
        tabs.addTab("Cross Mode", panel);
        panel = new ModeConfigurationPanel(OperationMode.CRISS);
        modes.put(panel.getOperationMode(), panel);
        tabs.addTab("Criss Mode", panel);
        if(enableSetupMode){
            panel = new ModeConfigurationPanel(OperationMode.SETUP);
            modes.put(panel.getOperationMode(), panel);
            tabs.addTab("Setup Mode", panel);
        }

        add(tabs);

        setup(OperationMode.CROSS, true, false, false);
        setup(OperationMode.CROSS, SensorType.X_SENSOR, "Unassigned");
        setup(OperationMode.CROSS, SensorType.Y_SENSOR, "Control Change", 1, Y_NOTES_CC, 0, 127);
        setup(OperationMode.CROSS, SensorType.POT_SENSOR, "Control Change", 1, POT_CC, 0, 127);

        setup(OperationMode.CRISS, false, false, false);
        setup(OperationMode.CRISS, SensorType.X_SENSOR, "Control Change", 1, X_CC, 0, 127);
        setup(OperationMode.CRISS, SensorType.Y_SENSOR, "Control Change", 1, Y_CC, 0, 127);
        setup(OperationMode.CRISS, SensorType.POT_SENSOR, "Control Change", 1, POT_CC, 0, 127);

        if(enableSetupMode){
            setup(OperationMode.SETUP, false, false, false);
            setup(OperationMode.SETUP, SensorType.X_SENSOR, "Basenote");
            setup(OperationMode.SETUP, SensorType.Y_SENSOR, "Scale");
            setup(OperationMode.SETUP, SensorType.POT_SENSOR, "Unassigned");
        }
    }

    public void setup(OperationMode mode, SensorType sensor, String type){
        setup(mode, sensor, type, 1, 1, 0, 127);
    }

    public void setup(OperationMode mode, SensorType sensor, String type, int channel, int cc, int min, int max){
        ModeConfigurationPanel panel = modes.get(mode);
        panel.setup(sensor, type, channel, cc, min, max);
    }

    public void setup(OperationMode mode, boolean doPlay, boolean doPb, boolean doAt){
        ModeConfigurationPanel panel = modes.get(mode);
        panel.setup(doPlay, doPb, doAt);
    }


    public static void main(String[] args)
        throws Exception {
        MidiConfigurationCanvas canvas = new MidiConfigurationCanvas(null, null);        
        JFrame frame = new JFrame();
        frame.add(canvas);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 400);
        frame.setVisible(true);
    }


}