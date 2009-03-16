package com.pingdynasty.blipbox;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.sound.midi.MidiUnavailableException;
import org.apache.log4j.Logger;

public class BlipBoxMidiApplication extends BlipBoxApplication {
    private static final Logger log = Logger.getLogger(BlipBoxMidiApplication.class);
    private MidiOutputEventHandler midihandler;
    private BlipBoxMidiConfiguration midiconfig;

    public BlipBoxMidiApplication(String config)
        throws MidiUnavailableException {
        super(config);
        midihandler = new MidiOutputEventHandler(sender);
        midihandler.init();
        receiver.setSensorEventHandler(midihandler);
        MidiConfigurationCanvas canvas = new MidiConfigurationCanvas(midihandler, sender);
        JFrame frame = createFrame();
        frame.add(canvas);
        frame.setVisible(true);

        // setup midi configuration
        midiconfig = new BlipBoxMidiConfiguration();
        midiconfig.setUpdateAction(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    updateMidiDevices();
                }
            });

        midiconfig.init();
    }

    public JMenu createSettingsMenu(){
        JMenu menu = super.createSettingsMenu();
        JMenuItem item = new JMenuItem("MIDI");
        item.addActionListener(new AbstractAction(){
                public void actionPerformed(ActionEvent e){
                    try{
                        midiconfig.open();
                    }catch(Exception exc){log.error("Failed to open MIDI configuration", exc);}
                }
            });
        menu.add(item);
        return menu;
    }

    public void updateMidiDevices(){
        try{
            midihandler.setMidiPlayer(midiconfig.getMidiOutput());
            midihandler.setChannel(midiconfig.getChannel());
        }catch(Exception exc){log.error(exc);}
    }
}
