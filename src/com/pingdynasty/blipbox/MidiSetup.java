package com.pingdynasty.blipbox;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.sound.midi.MidiUnavailableException;
import de.humatic.mmj.MidiSystem;
import com.pingdynasty.midi.SpringUtilities;

// this is currently using mmj libs but could be changed to use different MidiPlayer
// and run off javax.midi system

public class MidiSetup {
    private String[] inputnames;
    private String[] outputnames;
    private Map<String, MidiPlayer> devices;
    private JFrame frame;
    private Action updateAction; // action to perform when configuration is saved/updated
    private Action cancelAction; // action to perform when configuration is cancelled

    class DeviceActionListener implements ActionListener {
        private String device;
        private String name;
        private boolean input;

        public DeviceActionListener(String device, String name, boolean input){
            this.device = device;
            this.name = name;
        }

        public void actionPerformed(ActionEvent event) {
            MidiPlayer previous = getDevice(name);
            if(previous != null && previous.getName().equals(device))
                return;
            if(previous != null)
                previous.close();
            if(device == null){
                devices.remove(name);
                System.out.println(name+": disabled");
            }else{
                try{
                    MidiPlayer current = input ? openInputDevice(device) : openOutputDevice(device);
                    setDevice(name, current);
                    System.out.println(name+": "+current.getName());
                }catch(MidiUnavailableException exc){
                    exc.printStackTrace();
                }
            }
        }
    }

    public MidiSetup(String[] inputnames, String[] outputnames){
        this.inputnames = inputnames;
        this.outputnames = outputnames;
        devices = new HashMap<String, MidiPlayer>();
    }

    public void setUpdateAction(Action updateAction){
        this.updateAction = updateAction;
    }

    public void setCancelAction(Action cancelAction){
        this.cancelAction = cancelAction;
    }

    public void init()
        throws MidiUnavailableException {
    }

    public JPanel getPanel()
        throws MidiUnavailableException {
        JPanel content = new JPanel();
        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        JTabbedPane tabs = new JTabbedPane();
        JComponent panel = getInputPanel();
        if(panel != null)
            tabs.addTab("MIDI Input", panel);
        panel = getOutputPanel();
        if(panel != null)
            tabs.addTab("MIDI Output", panel);
        panel = getMiscPanel();
        if(panel != null)
            tabs.addTab("Miscellaneous", panel);
        //The following line enables to use scrolling tabs.
        tabs.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
        content.add(tabs);
        return content;
    }

    public JComponent getInputPanel()
        throws MidiUnavailableException {
        if(inputnames.length == 0)
            return null;
        return getPanel(inputnames, true);
    }

    public JComponent getOutputPanel()
        throws MidiUnavailableException {
        if(outputnames.length == 0)
            return null;
        return getPanel(outputnames, false);
    }

    public JComponent getMiscPanel()
        throws MidiUnavailableException {
        return null;
    }

    public String[] getInputDeviceNames(){
        return MidiSystem.getInputs();
    }

    public String[] getOutputDeviceNames(){
        return MidiSystem.getOutputs();
    }

    public JPanel getPanel(String[] names, boolean input)
        throws MidiUnavailableException {
        String[] devicenames = input ? getInputDeviceNames() : getOutputDeviceNames();

        JPanel content = new JPanel(new SpringLayout());
        ButtonGroup[] groups = new ButtonGroup[names.length];
        for(int i=0; i<names.length; ++i)
            groups[i] = new ButtonGroup();

        // first row labels
        content.add(new JLabel());
        for(int i=0; i<names.length; ++i)
            content.add(new JLabel(names[i]));

        // second row 'none' buttons
        content.add(new JLabel("none"));
        for(int i=0; i<names.length; ++i){
            JRadioButton button = new JRadioButton();
            button.addActionListener(new DeviceActionListener(null, names[i], input));
            if(getDevice(names[i]) == null)
                button.setSelected(true);
            groups[i].add(button);
            content.add(button);
        }

        // device rows
        for(String device : devicenames){
            content.add(new JLabel(device));
            // input columns
            for(int i=0; i<names.length; ++i){
                JRadioButton button = new JRadioButton();
                button.addActionListener(new DeviceActionListener(device, names[i], input));
                MidiPlayer current = getDevice(names[i]);
                if(current != null && current.getName()
                   .equals(device))
                    button.setSelected(true);
                groups[i].add(button);
                content.add(button);
            }
        }
        SpringUtilities.makeCompactGrid(content, devicenames.length+2, names.length+1, 
                                        15, 15, 5, 5);
        return content;
    }

    public JFrame getFrame()
        throws MidiUnavailableException {
        if(frame == null){
            JComponent panel = getPanel();
            Box box = Box.createHorizontalBox();
            JButton button = new JButton("update");
            if(updateAction != null)
                button.addActionListener(updateAction);
            button.addActionListener(new AbstractAction(){
                    public void actionPerformed(ActionEvent event){
                        frame.setVisible(false);
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
            frame = new JFrame("MIDI configuration");
            frame.setContentPane(panel);
            frame.pack();
        }
        return frame;
    }

    public void setDevice(String name, MidiPlayer device){
        devices.put(name, device);
    }

    public MidiPlayer getDevice(String name){
        return devices.get(name);
    }

    public MidiPlayer openInputDevice(String name)
        throws MidiUnavailableException {
        String[] devicenames = MidiSystem.getInputs();
        MidiPlayer player = null;
        for(int i=0; player == null && i<devicenames.length; ++i)
            if(devicenames[i].equals(name))
                throw new RuntimeException("Method not implemented - todo!");
//                     input = new MidiInputPlayer(MidiSystem.openMidiInput(i));
        return player;
    }

    public MidiPlayer openOutputDevice(String name)
        throws MidiUnavailableException {
        String[] devicenames = MidiSystem.getOutputs();
        MidiPlayer player = null;
        for(int i=0; player == null && i<devicenames.length; ++i)
            if(devicenames[i].equals(name))
                player = new MidiOutputPlayer(MidiSystem.openMidiOutput(i));
        return player;
    }

    public void open()
        throws MidiUnavailableException {
        if(frame == null)
            getFrame();
        frame.setVisible(true);
    }

    public void close(){
        frame.setVisible(false);
    }
}