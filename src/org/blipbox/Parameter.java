package org.blipbox;

public enum Parameter {

    BRIGHTNESS             (0x01<<2, "Brightness"),
    SENSITIVITY            (0x02<<2, "Sensitivity"),
    TLC_GSCLK_PERIOD       (0x03<<2, "TLC GSCLK period"),
    SERIAL_SPEED           (0x04<<2, "Serial speed"),
    FOLLOW_MODE            (0x05<<2, "Follow mode"),
    MIDI_ZONE              (0x06<<2, "MIDI zone data"),
    X_MIN                  (0x07<<2, "Touchscreen X minimum position"),
    X_RANGE                (0x08<<2, "Touchscreen X range"),
    Y_MIN                  (0x09<<2, "Touchscreen Y minimum position"),
    Y_RANGE                (0x0a<<2, "Touchscreen Y range");

    public int pid;
    public String name;

    private Parameter(int pid, String name){
        this.pid = pid;
        this.name = name;
    }

    static public Parameter getParameterForId(int pid){
        for(Parameter param : Parameter.values())
            if(param.pid == pid)
                return param;
        return null;
    }
}
