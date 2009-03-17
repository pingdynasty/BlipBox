package com.pingdynasty.midi;

import java.util.List;
import java.util.ArrayList;
import java.util.Properties;
import java.util.Enumeration;
import java.util.ResourceBundle;
import java.util.Locale;

public class ScaleMapper {

    private String[] scalenames = new String[]{
        "C Major",
        "C Minor Blues Scale",
        "C Major Blues Scale",
        "Ionian Mode",
        "Dorian Mode",
        "Phrygian Mode",
        "Lydian Mode",
        "Mixolydian Mode",
        "Aeolian Mode",
        "Locrian Mode",
        "Chromatic Scale"
    };

    private int[][] scales = new int[][]{
        // C Major
        {0,2,4,5,7,9,11},
        // C minor blues scale: C Eb F F# G Bb C
        {0, 3, 5, 6, 7, 10},
        // C major blues scale: C D D# E G A C
        {0, 2, 3, 4, 7, 9},
        // the seven modes of the diatonic major scale and added-note scales.
        // Ionian mode 	C D E F G A B C 	(associated with C Major 7 chord)
        {0,2,4,5,7,9,11},
        // Dorian mode 	C D Eb F G A Bb C 	(associated with C-7 chord)
        {0,2,3, 5,7,9,10},
        // Phrygian mode C Db Eb F G Ab Bb C 	(associated with C Phrygian chord)
        {0,1, 3, 5,7,8,10},
        // Lydian mode 	C D E F# G A B C 	(associated with C Maj7 #4 chord)
        {0,2,4,6, 7,9,11},
        // Mixolydian mode C D E F G A Bb C 	(associated with C7 chord)
        {0,2,4,5,7,9,10},
        // Aeolian mode D Eb F G Ab Bb C 	(associated with C-7 b6 chord)
        {2,3, 5,7,8, 10},
        // Locrian mode	C Db Eb F Gb Ab Bb C 	(associated with C-7b5 chord)
        {0,1, 3, 5,6, 8, 10},
        // Chromatic Scale
        {0,1,2,3,4,5,6,7,8,9,10,11}
    };
    int scaleindex = 0;

    public ScaleMapper(){}

    public ScaleMapper(Locale locale){
        ResourceBundle bundle = 
            ResourceBundle.getBundle("com.pingdynasty.midi.ScaleMapper", locale);
        map(bundle);
    }

    public void map(ResourceBundle bundle){
        List scalesList = new ArrayList();
        List namesList = new ArrayList();
        for(Enumeration e = bundle.getKeys(); e.hasMoreElements();){
            String name = (String)e.nextElement();
            namesList.add(name);
            String[] notes = bundle.getString(name).split(" ");
            int scale[] = new int[notes.length];
            for(int i=0; i<notes.length; ++i)
                scale[i] = NoteParser.getMidiNote(notes[i]) % 12; // should be in lowest octave
            scalesList.add(scale);
        }
        scales = new int[scalesList.size()][];
        scalesList.toArray(scales);
        scalenames = new String[namesList.size()];
        namesList.toArray(scalenames);
    }

//     public void map(Properties props){
//         for(Enumeration e = props.propertyNames(); e.hasMoreElements();){
//             String key = (String)e.nextElement();
//             int note = NoteParser.getMidiNote(props.getProperty(key));
//             int keycode = KeyStroke.getKeyStroke(key).getKeyCode();
//             mappings[keycode] = note;
//         }
//     }

    public String[] getScaleNames(){
        return scalenames;
    }

    public int getScaleIndex(){
        return scaleindex;
    }

    public void setScale(int scaleindex){
        this.scaleindex = scaleindex;
    }

    public void setScale(String name){
        int i=0;
        for(; i<scales.length && !scalenames[i].equals(name); ++i);
        if(i == scales.length)
            throw new IllegalArgumentException("no such scale: "+name);
        scaleindex = i;
    }

    /** map an arbitrary number to a MIDI note on the current scale */
    public int getNote(int key){
        if(key < 0)
            return -1;
        int note = scales[scaleindex][key % scales[scaleindex].length];
        note += (key / scales[scaleindex].length) * 12;
        // 12 is the length of an octave in midi (half) notes
        if(note > 127)
            return -1;
        return note;
    }

    /** approximate reverse mapping from MIDI note (though not all notes are represented on all scales) to key */
    public int getKey(int note){
        int key  = note % 12;
        key += (note / 12) * scales[scaleindex].length;
        return key;
    }

    public static void main(String[] args){
        ScaleMapper mapper = new ScaleMapper(Locale.getDefault());
    }
}
