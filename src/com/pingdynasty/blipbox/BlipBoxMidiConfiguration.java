package com.pingdynasty.blipbox;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.sound.midi.*;
// import com.pingdynasty.midi.*;
// import de.humatic.mmj.MidiSystem;

public class BlipBoxMidiConfiguration extends MidiSetup  {

    private final static String midiOutputName = "Output Interface";
    private int channel = 1;

    public BlipBoxMidiConfiguration(){
        super(new String[]{},  
              new String[]{midiOutputName});
    }

    public int getChannel(){
        return channel;
    }

    public void init()
        throws MidiUnavailableException {
        // initialise MIDI out
        String virtualSource = "BlipBox Output";
        // initialise midi and create a virtual source for the application
        de.humatic.mmj.MidiSystem.initMidiSystem(virtualSource, null);
        setDevice(midiOutputName, openOutputDevice(virtualSource));
    }

    public MidiPlayer getMidiOutput(){
        return getDevice(midiOutputName);
    }

//     public JComponent getMiscPanel()
//         throws MidiUnavailableException {
//         Box content = Box.createHorizontalBox();
// ...
//         return content;
//     }

    public JPanel getPanel()
        throws MidiUnavailableException{
        JPanel panel = super.getPanel();

        // channel configuration
        JPanel combo = new JPanel();
        JSpinner spinner = new JSpinner(new SpinnerNumberModel(channel, 1, 16, 1));
        spinner.addChangeListener(new ChangeListener(){
                public void stateChanged(ChangeEvent e){
                    JSpinner source = (JSpinner)e.getSource();
                    channel = ((Integer)source.getValue()).intValue();
                }
            });
        combo.add(new JLabel("MIDI channel:"));
        combo.add(spinner);
        panel.add(combo);

        return panel;
    }

}