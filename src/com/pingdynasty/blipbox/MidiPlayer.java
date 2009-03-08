package com.pingdynasty.blipbox;

import javax.sound.midi.InvalidMidiDataException;

public abstract class MidiPlayer {
    protected int channel = 0;

    /**
     * Play note with predetermined duration and velocity.
     * This implements a blocking call to play note - waits for @duration milliseconds
     */
    public void play(int note, int velocity, int duration)
        throws InvalidMidiDataException{
        noteOn(note, velocity);
        try{
            Thread.sleep(duration);
        }catch(InterruptedException exc){}
        noteOff(note);
    }

    public void setChannel(int channel){
        this.channel = channel;
    }

    public int getChannel(){
        return channel;
    }

    public void allNotesOff()
        throws InvalidMidiDataException{
        controlChange(123, 0);
    }

    public void modulate(int degree)
        throws InvalidMidiDataException{
        controlChange(1, degree);
    }

    public abstract void noteOn(int note, int velocity)
        throws InvalidMidiDataException;
    public abstract void noteOff(int note)
        throws InvalidMidiDataException;
    /** bend - the amount of pitch change, as a nonnegative 14-bit value (8192 = no bend) (0-16384) */
    public abstract void pitchBend(int degree)
        throws InvalidMidiDataException;
    public abstract void programChange(int bank, int program)
        throws InvalidMidiDataException;
    public abstract void controlChange(int code, int value)
        throws InvalidMidiDataException;
    public abstract String getName();
    public abstract void close();
}
