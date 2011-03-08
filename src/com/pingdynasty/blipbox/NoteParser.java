package com.pingdynasty.blipbox;

/**
 * Converts to and from string representations of notes and their 
 * MIDI equivalents.
 * Represents middle C (MIDI note 60) as C4, ie C in the fourth octave.
 * MIDI note 0 is represented as C-1, C in the -1 octave.
 * When converting from a string representation, halfnotes can be denoted 
 * as either sharp or flat. When converting to string, halfnotes are 
 * always represented as sharp.
 * Hence MIDI note 1 is represented as C#-1, C# (or Db) in the -1 octave.
 */
public class NoteParser {

    private static final String[] sharps = new String[] {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };
    private static final String[] flats = new String[] {
        "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
    };

//     public char[] notes = new char[]{
//         'C', 'D', 'E', 'F', 'G', 'A', 'B'
//     }

    /** Parses a note, eg C, C# or Cb, 
     * optionally followed by an indication of octave,
     * eg C0, C#1.
     * Note that notes must be in uppercase, eg C, not c.
     * returns the MIDI note equivalent, starting at 0 for C
     */
    public static int getMidiNote(String str){
        int octave = 0;
        StringBuffer buf = new StringBuffer();
        for(int i=0; i<str.length() && (str.charAt(i) > 57 || str.charAt(i) < 48); ++i)
            buf.append(str.charAt(i));
        String tone = buf.toString();
        if(tone.length() < str.length())
            octave = Integer.parseInt(str.substring(tone.length()));
        int note = -1;
        for(int i=0; i<12 && note == -1; ++i)
            if(sharps[i].equals(tone))
                note = i;
            else if(flats[i].equals(tone))
                note = i;
        if(note == -1)
            return -1;
        return note + ++octave * 12;
    }

    public static String getStringNote(int note){
        int octave = note / 12 - 1;
        return sharps[note % 12] + octave;
    }

    public static void main(String[] args){
        for(int i=0; i<args.length; ++i)
            System.out.println(args[i]+"\t"+getMidiNote(args[i])
                               +"\t"+getStringNote(getMidiNote(args[i])));
    }
}