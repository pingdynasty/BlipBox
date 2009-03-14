package com.pingdynasty.blipbox;

import javax.sound.midi.*;
import de.humatic.mmj.MidiOutput;
import de.humatic.mmj.MidiSystem;
import javax.sound.midi.InvalidMidiDataException;

public class MidiOutputPlayer extends MidiPlayer {
    protected MidiOutput midiout;

    public MidiOutputPlayer(int index){
        midiout = MidiSystem.openMidiOutput(index);
    }

    public MidiOutputPlayer(MidiOutput midiout){
        this.midiout = midiout;
    }

    public void setMidiOutput(MidiOutput midiout){
        this.midiout = midiout;
    }

    protected void midiMessage(int msgid, int first, int second)
        throws InvalidMidiDataException{
        byte header = (byte)((msgid << 4) | ((channel-1) & 0xf));
        try{
            midiout.sendMidi(new byte[] {header, (byte)first, (byte)second}, MidiSystem.getHostTime());
        }catch(Exception exc){
            throw new InvalidMidiDataException(exc.getMessage());
        }
    }

    protected void midiMessage(int msgid, int data)
        throws InvalidMidiDataException{
        byte header = (byte)((msgid << 4) | ((channel-1) & 0xf));
        try{
            midiout.sendMidi(new byte[] {header, (byte)data}, MidiSystem.getHostTime());
        }catch(Exception exc){
            throw new InvalidMidiDataException(exc.getMessage());
        }
    }

    public String getName(){
        return midiout.getName();
    }

    public void noteOn(int note, int velocity)
        throws InvalidMidiDataException{
        midiMessage(0x9, note, velocity);
    }

    public void noteOff(int note)
        throws InvalidMidiDataException{
        midiMessage(0x8, note, 0);
    }

    /** bend - the amount of pitch change, as a nonnegative 14-bit value (8192 = no bend) */
    public void pitchBend(int degree)
        throws InvalidMidiDataException{
        midiMessage(0xe, degree & 0x7F, (degree >> 7) & 0x7F);
    }

    public void controlChange(int code, int value)
        throws InvalidMidiDataException{
        midiMessage(0xb, code, value);
    }

    public void programChange(int bank, int program)
        throws InvalidMidiDataException{
        midiMessage(0xc, program);
    }

    public void channelPressure(int value)
        throws InvalidMidiDataException{
        midiMessage(0xd, value);
    }

    public void afterTouch(int note, int value)
        throws InvalidMidiDataException{
        midiMessage(0xa, note, value);
    }

    public void close(){
    }
}
