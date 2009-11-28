package org.blipbox.midimodes;

import rwmidi.*;
import org.blipbox.*;

public class ControlChangeMode extends AbstractMidiMode {
    private int cc1;
    private int cc2;

    public ControlChangeMode(int cc1, int cc2){
        this.cc1 = cc1;
        this.cc2 = cc2;
    }

    public void position(Position pos){
        log.debug("cc "+pos);
        output.sendController(channel, cc1, pos.getX(0, 127));
        output.sendController(channel, cc2, pos.getY(0, 127));
    }
}

