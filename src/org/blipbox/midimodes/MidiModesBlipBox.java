package org.blipbox.midimodes;

import java.io.IOException;
import org.blipbox.*;

public class MidiModesBlipBox extends BlipBox {

    private MidiModesAction action;

    public MidiModesBlipBox() {
        action = new MidiModesAction(this);
        addInputHandler(action);
    }

    public void start(){
        long clock = 0;
        for(;;)
            action.tick(clock);
    }

    public void dispose() {
        closeSerialPort();
    }

}