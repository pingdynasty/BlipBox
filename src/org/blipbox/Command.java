package org.blipbox;

public enum Command {

    FADE                (0x04, "Decrease LED brightness"),
    BRIGHTEN            (0x05, "Increase LED brightness"),
    CFG_REQUEST         (0x06, "Request configuration parameters"),
    CFG_RESET           (0x07, "Reset configuration"),
    CFG_READ            (0x08, "Read configuration from EEPROM"),
    CFG_WRITE           (0x09, "Write configuration to EEPROM"),
    LEDS_STOP           (0x10, "Stop LED screen updates"),
    LEDS_START          (0x11, "Start LED screen updates");

    public int cid;
    public String name;

    Command(int cid, String name){
        this.cid = cid;
        this.name = name;
    }
}
