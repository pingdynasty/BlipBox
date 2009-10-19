package org.blipbox.midimodes;

import rwmidi.*;
import org.blipbox.*;

public class NotePlayerMode extends AbstractMidiMode {
    int tone;
    int velocity;
    int root = 60;
    int range = 20;
    int channel = 0;

    public void tap(Position pos){
        tone = pos.getX(root, root+range);
        log.debug("note player on: "+tone);
        output.sendNoteOn(channel, tone, pos.getY(0, 127));
    }

    public void release(Position pos){
        log.debug("note player off: "+tone);
        output.sendNoteOff(channel, tone, 0);
    }
}
