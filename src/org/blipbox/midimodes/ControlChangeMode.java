package org.blipbox.midimodes;

import rwmidi.*;
import org.blipbox.*;

public class ControlChangeMode extends AbstractMidiMode {
    int cc1;
    int cc2;
    int cc1value = 64;
    int cc2value = 64;

    public ControlChangeMode(int cc1, int cc2){
        this.cc1 = cc1;
        this.cc2 = cc2;
    }

    public void drag(int x, int y, int dx, int dy){
        log.debug("cc drag");
        cc1value = normalize(x - dx, cc1value);
        output.sendController(channel, cc1, cc1value);
        cc2value = normalize(x - dx, cc2value);
        output.sendController(channel, cc2, cc2value);
    }

    private int normalize(int difference, int value){
        if(difference > 10){
            if(++value > 127)
                value = 127;
        }
        else if(difference < -10){
            if(--value < 0)
                value = 0;
        }
        return value;
    }
}

