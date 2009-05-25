package com.pingdynasty.blipbox;

import java.util.Map;
import java.util.HashMap;
import javax.sound.midi.*;
import com.pingdynasty.midi.ScaleMapper;
import org.apache.log4j.Logger;

public class MidiOutputEventHandler extends MultiModeKeyPressManager {
    private static final Logger log = Logger.getLogger(MidiOutputEventHandler.class);

    private static final int OCTAVE_SHIFT = 6;
    private MidiPlayer midiPlayer;
    private int lastNote = 0;

    public class MidiConfigurationMode extends ConfigurationMode {
        private ScaleMapper mapper;
        private int basenote = 40;

        public MidiConfigurationMode(String name, String follow){
            super(name, follow);
            mapper = new ScaleMapper();
            mapper.setScale("Mixolydian Mode");
//             setScale("Chromatic Scale");
//         setScale("C Major");
//         setScale("Dorian Mode");
        }

        public ScaleMapper getScaleMapper(){
            return mapper;
        }

        public int getBaseNote(){
            return basenote;
        }

        public void setBaseNote(int basenote){
            this.basenote = basenote;
        }
    }

    public ConfigurationMode createConfigurationMode(String mode, String follow){
        return new MidiConfigurationMode(mode, follow);
    }

    public ScaleMapper getScaleMapper(){
        MidiConfigurationMode mode = (MidiConfigurationMode)getCurrentConfigurationMode();
        return mode.getScaleMapper();
    }

    public ScaleMapper getScaleMapper(String mode){
        MidiConfigurationMode config = (MidiConfigurationMode)getConfigurationMode(mode);
        return config.getScaleMapper();
    }

    public int getBaseNote(){
        MidiConfigurationMode mode = (MidiConfigurationMode)getCurrentConfigurationMode();
        return mode.getBaseNote();
    }

    public void setBaseNote(int basenote){
        MidiConfigurationMode mode = (MidiConfigurationMode)getCurrentConfigurationMode();
        mode.setBaseNote(basenote);
    }

    public void setBaseNote(String modename, int basenote){
        MidiConfigurationMode mode = (MidiConfigurationMode)getConfigurationMode(modename);
        mode.setBaseNote(basenote);
    }

    public void holdOff(){
        super.holdOff();
        sendMidiNoteOff(lastNote);
    }

    public void init(){
        super.init();
        setSensorEventHandler("Cross", SensorType.BUTTON2_SENSOR, new OctaveShiftUpEventHandler());
        setSensorEventHandler("Cross", SensorType.BUTTON3_SENSOR, new OctaveShiftDownEventHandler());
        setSensorEventHandler("Criss", SensorType.BUTTON2_SENSOR, new OctaveShiftUpEventHandler());
        setSensorEventHandler("Criss", SensorType.BUTTON3_SENSOR, new OctaveShiftDownEventHandler());
    }

    public class OctaveShiftUpEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0){
                int basenote = getBaseNote();
                log.debug("octave up");
                basenote += OCTAVE_SHIFT;
                if(basenote + OCTAVE_SHIFT <= 127)
                    setBaseNote(basenote);
                log.debug("new basenote "+basenote);
            }
        }
    }

    public class OctaveShiftDownEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0){
                int basenote = getBaseNote();
                log.debug("octave down");
                basenote -= OCTAVE_SHIFT;
                if(basenote > 0)
                    setBaseNote(basenote);
                log.debug("new basenote "+basenote);
            }
        }
    }

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
            int basenote = sensor.scale(min, max);
            log.debug("basenote: "+basenote);
            setBaseNote(basenote);
        }
    }

    public class ScaleChangeEventHandler implements SensorEventHandler {
        public void sensorChange(SensorDefinition sensor){
            ScaleMapper mapper = getScaleMapper();
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

    public class NonRegisteredParameterEventHandler implements SensorEventHandler {
        private int from;
        private int to;
        private int cc;

        public NonRegisteredParameterEventHandler(int cc, int from, int to){
            this.from = from;
            this.to = to;
            this.cc = cc;
        }

        public void sensorChange(SensorDefinition sensor){
            int val = sensor.scale(from, to);
            sendMidiNRPN(cc, val);
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
//             int velocity = (row*127/8)+1;
            int velocity = ((row+1)*127/9);
            return velocity;
        }

        public void keyDown(int col, int row){
            lastNote = getScaleMapper().getNote(col+getBaseNote());
            sendMidiNoteOn(lastNote, getVelocity(row));
        }

        public void keyUp(int col, int row){
            sendMidiNoteOff(lastNote);
        }

        public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
            int newNote = getScaleMapper().getNote(newCol+getBaseNote());
            if(newNote != lastNote){
                sendMidiNoteOff(lastNote);
                sendMidiNoteOn(newNote, getVelocity(newRow));
            }
            lastNote = newNote;
        }
    }

    public MidiOutputEventHandler(BlipBox sender){
        super(sender);
    }

    public void configureControlChange(String mode, SensorType type, int channel, int cc, int min, int max){
        log.debug("Setting "+mode+":"+type+" to CC "+cc+" ("+min+"-"+max+")");
        setSensorEventHandler(mode, type, new ControlChangeEventHandler(cc, min, max));
    }

    public void configureNRPN(String mode, SensorType type, int channel, int cc, int min, int max){
        log.debug("Setting "+mode+":"+type+" to NRPN "+cc+" ("+min+"-"+max+")");
        setSensorEventHandler(mode, type, new NonRegisteredParameterEventHandler(cc, min, max));
    }

    public void configurePitchBend(String mode, SensorType type, int channel, int min, int max){
        setSensorEventHandler(mode, type, new PitchBendEventHandler(min, max));
    }

    public void configureBaseNoteChange(String mode, SensorType type, int min, int max){
        log.debug("Setting "+mode+":"+type+" to control base note");
        setSensorEventHandler(mode, type, new BaseNoteChangeEventHandler(min, max));
    }

    public void configureScaleChange(String mode, SensorType type){
        log.debug("Setting "+mode+":"+type+" to control scale changes");
        setSensorEventHandler(mode, type, new ScaleChangeEventHandler());
    }

    public void configureNotePlayer(String mode, boolean notes, boolean pb, boolean at){
        log.debug("Setting "+mode+" mode to play notes ("+notes+") pitch bend ("+pb+") aftertouch ("+at+")");
        if(notes){
            setKeyEventHandler(mode, new NotePlayer());
        }else{
            setKeyEventHandler(mode, null);
        }
        // todo: honour pb and at
    }

//     public String[] getScaleNames(){
//         return mapper.getScaleNames();
//     }

//     public void setScale(int index){
//         mapper.setScale(index);
//     }

    public String getCurrentScale(){
        ScaleMapper mapper = getScaleMapper();
        return mapper.getScaleNames()[mapper.getScaleIndex()];
    }

    public void setMidiPlayer(MidiPlayer midiPlayer){
        this.midiPlayer = midiPlayer;
    }

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
            if(midiPlayer != null)
                midiPlayer.noteOn(note, velocity);
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
            if(midiPlayer != null)
                midiPlayer.noteOff(note);
        }catch(Exception exc){
            log.error(exc, exc);
        }        
    }


    public void sendMidiNRPN(int parameter, int value){
//         log.debug("nrpn ("+parameter+") :\t "+value);
        sendMidiCC(99, 3);
        sendMidiCC(98, parameter & 0x7f);   // NRPN LSB
        sendMidiCC(6, value);

//         sendMidiCC(99, parameter >> 7);     // NRPN MSB
//         sendMidiCC(98, parameter & 0x7f);   // NRPN LSB
//         sendMidiCC(6, value >> 7);          // Data Entry MSB
//         if((value & 0x7f) != 0)
//             sendMidiCC(38, value & 0x7f);   // Data Entry LSB
    }

    public void sendMidiCC(int cc, int value){
//         log.debug("midi cc:\t "+cc+"\t  "+value);
        if(value > 127 || value < 0){
            log.error("MIDI CC "+cc+" value out of range: "+value);
            return;
        }
        try {
            if(midiPlayer != null)
                midiPlayer.controlChange(cc, value);
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
            if(midiPlayer != null)
                midiPlayer.pitchBend(degree);
        }catch(Exception exc){
            log.error(exc, exc);
        }        
    }

    public void setChannel(int channel){
        midiPlayer.setChannel(channel);
    }

    public class SensitiveNotePlayer implements KeyEventHandler {
        private int lastnote;
        private int velocity;

        // todo : velocity could be set by row rather than sensor position
        public void sensorChange(SensorDefinition sensor){
            velocity = sensor.scale(127);
        }

        public void keyDown(int col, int row){
            lastNote = getScaleMapper().getNote(col+getBaseNote());
            sendMidiNoteOn(lastNote, velocity);
        }

        public void keyUp(int col, int row){
            sendMidiNoteOff(lastNote);
        }

        public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
            int newNote = getScaleMapper().getNote(newCol+getBaseNote());
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