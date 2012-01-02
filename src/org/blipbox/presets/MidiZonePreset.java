package org.blipbox.presets;

public class MidiZonePreset {
    public static final byte READ_PRESET_COMMAND    = 1 << 4;
    public static final byte REQUEST_PRESET_COMMAND = 2 << 4;
    private int index;
    private MidiZone[] zones;

    public MidiZonePreset(){
	zones = new MidiZone[8];
	for(int i=0; i<zones.length; ++i)
	    zones[i] = new MidiZone();
    }

    public MidiZonePreset(int index, MidiZone[] zones){
	this.index = index;
	this.zones = zones;
    }

    public int getIndex(){
	return index;
    }

    public void setIndex(int index){
	this.index = index;
    }

    public MidiZone getZone(int index){
	return zones[index];
    }

//     public byte[] serialize(){
// 	for(int i=0; i<zones.length; ++i){
// 	}
//     }


    public String toString(){
	String str = "MidiZonePreset["+index+"]{";
	for(int i=0; i<zones.length; ++i)
	    str += "\n"+zones[i].toString();
	str += "}";
	return str;
    }

}
