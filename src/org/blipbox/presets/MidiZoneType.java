package org.blipbox.presets;

public enum MidiZoneType {
        HORIZONTAL_CC      (1),
	VERTICAL_CC        (2),
	CC_PUSH_BUTTON     (1 | 1<<MidiZone.BUTTON_ZONE_BIT),
	CC_TOGGLE_BUTTON   (2 | 1<<MidiZone.BUTTON_ZONE_BIT),
	HORIZONTAL_NOTE    (1 | 1<<MidiZone.NOTE_ZONE_BIT),
	VERTICAL_NOTE      (2 | 1<<MidiZone.NOTE_ZONE_BIT),
	NOTE_PUSH_BUTTON   (1 | 1<<MidiZone.NOTE_ZONE_BIT | 1<<MidiZone.BUTTON_ZONE_BIT),
	NOTE_TOGGLE_BUTTON (2 | 1<<MidiZone.NOTE_ZONE_BIT | 1<<MidiZone.BUTTON_ZONE_BIT),
	PRESET_BUTTON      (0);
    //         HORIZONTAL_CC       (1<<4 | 1<<6 | 1),
    //         VERTICAL_CC         (1<<4 | 1<<6 | 2),
    //         HORIZONTAL_NOTE     (1<<5 | 1<<6 | 3),
    //         VERTICAL_NOTE       (1<<5 | 1<<6 | 4),
    //         NOTE_PUSH_BUTTON    (1<<5 | 1<<7 | 5),
    //         NOTE_TOGGLE_BUTTON  (1<<5 | 1<<7 | 6),
    //         CC_PUSH_BUTTON      (1<<4 | 1<<7 | 7),
    //         CC_TOGGLE_BUTTON    (1<<4 | 1<<7 | 8),
    //         PRESET_BUTTON       (              9);

    private int code;
    private MidiZoneType(int code){
	this.code = code;
    }
    public int getCode(){
	return code;
    }
}
