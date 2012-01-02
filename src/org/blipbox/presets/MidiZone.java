package org.blipbox.presets;

public class MidiZone {
    public MidiZoneType type;
    public int channel;
    public int data1;
    public int fromX, toX, fromY, toY;

    public static final int NOTE_ZONE_BIT   = 2;
    public static final int BUTTON_ZONE_BIT = 3;

    public MidiZone(){
	type = MidiZoneType.HORIZONTAL_CC;
    }

    public MidiZone(MidiZoneType type, int channel, int data1, 
		    int fromX, int toX, int fromY, int toY){
	this.type = type;
	this.channel = channel;
	this.data1 = data1;
	this.fromX = fromX;
	this.toX = toX;
	this.fromY = fromY;
	this.toY = toY;
    }

    public void write(int[] data){
	assert data.length == 4;
	data[0] = (type.getCode() << 4) | channel;
	data[1] = data1;
	data[2] = (fromX << 4) | fromY;
	data[3] = (toX << 4) | toY;
    }

    public void read(int[] data){
	assert data.length == 4;
	type = MidiZoneType.values()[data[0] >> 4];
	channel = data[0] & 0x0f;
	data1 = data[1];
	fromX = data[2] >> 4;
	fromY = data[2] & 0x0f;
	toX = data[3] >> 4;
	toY = data[3] & 0x0f;
    }

    public String toString(){
	return type+","+channel+","+data1+
	    ",["+fromX+","+fromY+"],["+toX+","+toY+"]";
    }
}
