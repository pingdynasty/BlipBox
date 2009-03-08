package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;
import javax.sound.midi.*;
import com.pingdynasty.midi.ScaleMapper;
import org.apache.log4j.Logger;

public class MidiOutputEventHandler extends KeyPressManager {
    private static final Logger log = Logger.getLogger(MidiOutputEventHandler.class);

    protected MidiPlayer midi;
    protected ScaleMapper mapper;
    private static final int Y_NOTES_CC = 18;
    private static final int X_CC = 20;
    private static final int Y_CC = 21;
    private static final int T_CC = 19;
    private static final int POT_CC = 1;
    private static final int BUTTON_CC = 22;
    private static final long DOUBLE_CLICK_MILLIS = 500;
//     private static final int OCTAVE_SHIFT = 12;
    private static final int OCTAVE_SHIFT = 6;
    private int lastNote = 0;
    private int basenote = 40;
    private boolean released; // flag to say if keys have been released since last mode switch
    private long lastButtonPress = 0; // must be shared between mode change event handlers (or event handlers must be shared)
    private boolean enableSetupMode = false;

    private OperationMode mode = OperationMode.CROSS;

    SensorHandlerConfiguration crossConfig = new SensorHandlerConfiguration();
    SensorHandlerConfiguration crissConfig = new SensorHandlerConfiguration();
    SensorHandlerConfiguration setupConfig = new SensorHandlerConfiguration();

    private KeyEventHandler currentConfig = crossConfig;

    {
        crossConfig.registerSensorEventHandler(SensorType.BUTTON2_SENSOR, new OctaveShiftUpEventHandler());
        crossConfig.registerSensorEventHandler(SensorType.BUTTON3_SENSOR, new OctaveShiftDownEventHandler());

        crissConfig.registerSensorEventHandler(SensorType.BUTTON2_SENSOR, new OctaveShiftUpEventHandler());
        crissConfig.registerSensorEventHandler(SensorType.BUTTON3_SENSOR, new OctaveShiftDownEventHandler());

        crossConfig.registerSensorEventHandler(SensorType.BUTTON1_SENSOR, new ModeChangeEventHandler());
        crissConfig.registerSensorEventHandler(SensorType.BUTTON1_SENSOR, new ModeChangeEventHandler());
        setupConfig.registerSensorEventHandler(SensorType.BUTTON1_SENSOR, new ModeChangeEventHandler());
    }

    public class OctaveShiftUpEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0){
                log.debug("octave up");
                if(basenote+(OCTAVE_SHIFT*2) <= 127)
                    basenote += OCTAVE_SHIFT;
                log.debug("new basenote "+basenote);
            }
        }
    }

    public class OctaveShiftDownEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0){
                log.debug("octave down");
                if(basenote-OCTAVE_SHIFT > 0)
                    basenote -= OCTAVE_SHIFT;
                log.debug("new basenote "+basenote);
            }
        }
    }

    public class ModeChangeEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0){
                long now = System.currentTimeMillis();
                if(enableSetupMode && now - lastButtonPress < DOUBLE_CLICK_MILLIS){
                    lastButtonPress = 0;
                    changeMode(OperationMode.SETUP);
                // todo: change led follow mode
                // todo: set led at basenote / scale position
                }else if(mode == OperationMode.CROSS){
                    changeMode(OperationMode.CRISS);
                // todo: change led follow mode
                    released = false;
                }else{
                    // hold off - unlock
                    sendMidiNoteOff(lastNote);
                    changeMode(OperationMode.CROSS);
                    released = false;
                }
                lastButtonPress = now;
            }
        }
    }

//     public class ModeChangeEventHandler implements SensorEventHandler {
//         private OperationMode mode;
//         public ModeChangeEventHandler(OperationMode mode){
//             this.mode = mode;
//         }
//         public void sensorChange(SensorDefinition sensor){
//             if(sensor.value != 0){
//                 log.debug("old mode: "+MidiOutputEventHandler.mode);
//                 changeMode(mode);
//                 log.debug("new mode: "+MidiOutputEventHandler.mode);
//             }
//         }
//     }

    public class BaseNoteChangeEventHandler implements SensorEventHandler {
        private int min, max;

        public BaseNoteChangeEventHandler(){
            this(0, 127);
        }

        public BaseNoteChangeEventHandler(int min, int max){
            this.min = min;
            this.max = max;
        }

        public void sensorChange(SensorDefinition sensor){
            basenote = sensor.scale(min, max);
            log.debug("basenote: "+basenote);
        }
    }

    public class ScaleChangeEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            int val = sensor.scale(mapper.getScaleNames().length);
            if(val < mapper.getScaleNames().length){
                mapper.setScale(val);
                log.debug("set scale "+mapper.getScaleNames()[val]);
            }
        }
    }

    public class ControlChangeEventHandler implements SensorEventHandler {
        private int from;
        private int to;
        private int cc;

        public ControlChangeEventHandler(int cc){
            this(cc, 0, 127);
        }

        public ControlChangeEventHandler(int cc, int from, int to){
            this.from = from;
            this.to = to;
            this.cc = cc;
        }

        public void sensorChange(SensorDefinition sensor){
            int val = sensor.scale(from, to);
            sendMidiCC(cc, val);
        }
    }

    public class PitchBendEventHandler implements SensorEventHandler {
        private int from;
        private int to;

        public PitchBendEventHandler(){

            this(-8191, 8192);
        }

        public PitchBendEventHandler(int from, int to){
            this.from = from;
            this.to = to;
        }

        public void sensorChange(SensorDefinition sensor){
            int val = sensor.scale(from, to);
            sendMidiPitchBend(val);
        }
    }

    public class NotePlayer implements KeyEventHandler {
        private int lastnote;

        public void sensorChange(SensorDefinition sensor){}
       
        protected int getVelocity(int row){
//             int velocity = ((row+1)*127/8);
            int velocity = (row*127/8);
            return velocity;
        }

        public void keyDown(int col, int row){
            lastNote = mapper.getNote(col+basenote);
            sendMidiNoteOn(lastNote, getVelocity(row));
        }

        public void keyUp(int col, int row){
            sendMidiNoteOff(lastNote);
        }

        public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
            int newNote = mapper.getNote(newCol+basenote);
            if(newNote != lastNote){
                sendMidiNoteOff(lastNote);
                sendMidiNoteOn(newNote, getVelocity(newRow));
            }
            lastNote = newNote;
        }
    }

    public MidiOutputEventHandler(){
        mapper = new ScaleMapper();
        setScale("Chromatic Scale");
//         setScale("C Major");
//         setScale("Dorian mode");
    }

    public void setKeyEventHandler(OperationMode mode, KeyEventHandler handler){
        switch(mode){
        case CROSS:
            crossConfig.registerKeyEventHandler(handler);
            break;
        case CRISS:
            crissConfig.registerKeyEventHandler(handler);
            break;
        case SETUP:
            setupConfig.registerKeyEventHandler(handler);
            break;
        default:
            throw new RuntimeException("Unknown operation mode "+mode);
        }
    }

    public void setSensorEventHandler(OperationMode mode, SensorType type, SensorEventHandler handler){
        switch(mode){
        case CROSS:
            crossConfig.registerSensorEventHandler(type, handler);
            break;
        case CRISS:
            crissConfig.registerSensorEventHandler(type, handler);
            break;
        case SETUP:
            setupConfig.registerSensorEventHandler(type, handler);
            break;
        default:
            throw new RuntimeException("Unknown operation mode "+mode);
        }
    }

    public void configureUnassigned(OperationMode mode, SensorType type){
        log.debug("Setting "+mode+":"+type+" to unassigned");
        setSensorEventHandler(mode, type, null);
    }

    public void configureControlChange(OperationMode mode, SensorType type, int channel, int cc, int min, int max){
        log.debug("Setting "+mode+":"+type+" to CC "+cc+" ("+min+"-"+max+")");
        setSensorEventHandler(mode, type, new ControlChangeEventHandler(cc, min, max));
    }

    public void configurePitchBend(OperationMode mode, SensorType type, int channel, int min, int max){
        setSensorEventHandler(mode, type, new PitchBendEventHandler(min, max));
    }

    public void configureBaseNoteChange(OperationMode mode, SensorType type, int min, int max){
        log.debug("Setting "+mode+":"+type+" to control base note");
        setSensorEventHandler(mode, type, new BaseNoteChangeEventHandler(min, max));
    }

    public void configureScaleChange(OperationMode mode, SensorType type){
        log.debug("Setting "+mode+":"+type+" to control scale changes");
        setSensorEventHandler(mode, type, new ScaleChangeEventHandler());
    }

    public void configureNotePlayer(OperationMode mode, boolean notes, boolean pb, boolean at){
        log.debug("Setting "+mode+" mode to play notes ("+notes+") pitch bend ("+pb+") aftertouch ("+at+")");
        if(notes){
            setKeyEventHandler(mode, new NotePlayer());
        }else{
            setKeyEventHandler(mode, null);
        }
        // todo: honour pb and at
    }

    public OperationMode getMode(){
        return mode;
    }

    public void changeMode(OperationMode mode){
        log.debug("set mode: "+mode);
        if(this.mode != mode){
            if(mode == OperationMode.CRISS)
                currentConfig = crissConfig;
            else if(mode == OperationMode.CROSS)
                currentConfig = crossConfig;
            else if(mode == OperationMode.SETUP)
                currentConfig = setupConfig;
            this.mode = mode;
        }
    }

    public int getBasenote(){
        return basenote;
    }

    public void setBasenote(int basenote){
        this.basenote = basenote;
    }

    public String[] getScaleNames(){
        return mapper.getScaleNames();
    }

    public void setScale(int index){
        mapper.setScale(index);
    }

    public void setScale(String scalename){
        mapper.setScale(scalename);
    }

    public String getCurrentScale(){
        return mapper.getScaleNames()[mapper.getScaleIndex()];
    }

    // event handlers

    public void sensorChange(SensorDefinition sensor){
        currentConfig.sensorChange(sensor);
        super.sensorChange(sensor); // trigger key events
    }

    public void keyDown(int col, int row){
        released = true;
        currentConfig.keyDown(col, row);
    }

    public void keyUp(int col, int row){
        released = true;
        currentConfig.keyUp(col, row);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        if(released){
            /// suppress keychange event if key has not been released since last mode switch (key hold)
            currentConfig.keyChange(oldCol, oldRow, newCol, newRow);
        }
    }

    public void setMidiPlayer(MidiPlayer midi){
        this.midi = midi;
    }

    private ShortMessage msg = new ShortMessage();

    public void sendMidiNoteOn(int note, int velocity){
        log.debug("note on:\t "+note+"\t  "+velocity);
        if(note > 127 || note < 0){
            log.error("MIDI note on "+note+"/"+velocity+" value out of range");
            return;
        }
        if(velocity > 127 || velocity < 0){
            log.error("MIDI note on "+note+"/"+velocity+" value out of range");
            velocity = velocity < 0 ? 0 : 127;
        }
        try {
            if(midi != null)
                midi.noteOn(note, velocity);
        }catch(Exception exc){
            log.error(exc, exc);
        }        
    }

    public void sendMidiNoteOff(int note){
//         note = mapper.getNote(note);
        log.debug("note off:\t "+note);
        if(note > 127 || note < 0){
            log.error("MIDI note off "+note+" value out of range");
            return;
        }
        try {
            if(midi != null)
                midi.noteOff(note);
        }catch(Exception exc){
            log.error(exc, exc);
        }        
    }

    public void sendMidiCC(int cc, int value){
//         log.debug("midi cc:\t "+cc+"\t  "+value);
        if(value > 127 || value < 0){
            log.error("MIDI CC "+cc+" value out of range: "+value);
            return;
        }
        try {
            if(midi != null)
                midi.controlChange(cc, value);
        }catch(Exception exc){
            log.error(exc, exc);
        }
    }

    public void sendMidiPitchBend(int degree){
        // send midi pitch bend in the range -8192 to 8191 inclusive
        if(degree < -8192 || degree > 8191){
            log.error("MIDI pitch bend value out of range: "+degree);
            return;
        }
        // setPitchBend() expects a value in the range 0 to 16383
        degree += 8192;
        try {
            if(midi != null)
                midi.pitchBend(degree);
        }catch(Exception exc){
            log.error(exc, exc);
        }        
    }

    public void setChannel(int channel){
        midi.setChannel(channel);
    }

    public class SensitiveNotePlayer implements KeyEventHandler {
        private int lastnote;
        private int velocity;

        // todo : velocity could be set by row rather than sensor position
        public void sensorChange(SensorDefinition sensor){
            velocity = sensor.scale(127);
        }

        public void keyDown(int col, int row){
            lastNote = mapper.getNote(col+basenote);
            sendMidiNoteOn(lastNote, velocity);
        }

        public void keyUp(int col, int row){
            sendMidiNoteOff(lastNote);
        }

        public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
            int newNote = mapper.getNote(newCol+basenote);
            if(newNote != lastNote){
                sendMidiNoteOff(lastNote);
                sendMidiNoteOn(newNote, velocity);
//                 }else{
//                     // todo: aftertouch, bend
            }
            lastNote = newNote;
        }
    }
}