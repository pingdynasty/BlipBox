package org.blipbox.midimodes;

import rwmidi.MidiOutput;
import org.blipbox.*;

public interface MidiMode extends BlipAction {
    public void setMidiChannel(int channel);
    public void setModeId(int modeId);
    public void setBlipBox(BlipBox blipbox);
    public void setMidiOutput(MidiOutput output);
    public int getModeId();
    public void start();
    public void stop();
    public void tick(long clock);

}