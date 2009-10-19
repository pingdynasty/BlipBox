package org.blipbox.midimodes;

import rwmidi.*;
import org.blipbox.*;
import org.apache.log4j.Logger;

public class AbstractMidiMode extends AbstractBlipAction implements MidiMode {
    private int modeId;
    protected static final Logger log = Logger.getLogger(AbstractMidiMode.class);
    protected BlipBox blipbox;
    protected MidiOutput output;
    protected int channel = 0;

    public void setBlipBox(BlipBox blipbox){
        this.blipbox = blipbox;
    }

    public void setMidiOutput(MidiOutput output){
        this.output = output;
    }

    public void setMidiChannel(int channel){
        this.channel = channel;
    }

    public void setModeId(int modeId){
        this.modeId = modeId;
    }

    public int getModeId(){
        return modeId;
    }

    public void start(){}
    public void stop(){}
    public void tick(long clock){}
}
