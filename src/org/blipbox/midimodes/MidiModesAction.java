package org.blipbox.midimodes;

import rwmidi.*;
import org.blipbox.*;
import org.apache.log4j.Logger;

public class MidiModesAction extends AbstractBlipAction {
    private static final Logger log = Logger.getLogger(MidiModesAction.class);
    private BlipBox blipbox;
    private MidiOutput output;
    private MidiMode mode;

    public class MidiMode extends AbstractBlipAction {
        private int modeId;
        public MidiMode(int modeId){
            this.modeId = modeId;
        }
        public int getModeId(){
            return modeId;
        }
        public void start(){}
        public void stop(){}
        public void tick(long clock){}
    }

    public MidiModesAction(MidiModesBlipBox blipbox){
        this.blipbox = blipbox;
        for(MidiOutputDevice device: RWMidi.getOutputDevices())
            log.info("MIDI output device: "+device.getName());
        this.output = RWMidi.getOutputDevices()[1].createOutput();
        log.info("Using MIDI output "+output.getName());
        mode = getMode(0);
    }

    public void tick(long clock){
        if(clock % 20 == 0){
            blipbox.fade();
            blipbox.setLed(9, mode.getModeId(), 0xff);
            if(blipbox.isScreenPressed())
                blipbox.setLed(blipbox.getX(0, 10), blipbox.getY(8, 0), 0xff);
        }
        mode.tick(clock);
    }

    public void tap(int x, int y){
        if(blipbox.getX(0, 10) == 9)
            changeMode(blipbox.getY(0, 8));
        else
            mode.tap(x, y);
    }

    public void taptap(int x, int y){
        mode.taptap(x, y);
    }

    public void release(int x, int y){
        mode.release(x, y);
    }

    public void drag(int x, int y, int dx, int dy){
        mode.drag(x, y, dx, dy);
    }

    public void position(int x, int y){
        mode.position(x, y);
    }

    public void changeMode(int modeId){
        if(mode.getModeId() != modeId){
            log.debug("Setting mode: "+modeId);
            mode.stop();
            mode = getMode(modeId);
            mode.start();
        }
    }

    public MidiMode getMode(int modeId){
        MidiMode mode;
        switch(modeId){
        case 0:
        case 1:
            mode = new NotePlayerMode(modeId);
            break;
        case 2:
        case 3:
            mode = new ControlChangeMode(modeId);
            break;
        default:
            mode = new MidiMode(modeId);
        }
        return mode;
    }

    public class NotePlayerMode extends MidiMode {
        int tone;
        int velocity;
        int root = 60;
        int range = 20;
        int channel = 0;

        public NotePlayerMode(int modeId){
            super(modeId);
        }
        public void tap(int x, int y){
            tone = blipbox.getX(root, root+range);
            log.debug("note player on: "+tone);
            output.sendNoteOn(channel, tone, blipbox.getY(0, 127));
        }
        public void release(int x, int y){
            log.debug("note player off: "+tone);
            output.sendNoteOff(channel, tone, 0);
        }
    }

    public class ControlChangeMode extends MidiMode {
        int channel = 0;
        int cc1 = 18;
        int cc2 = 19;
        int cc1value = 64;
        int cc2value = 64;

        public ControlChangeMode(int modeId){
            super(modeId);
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

}