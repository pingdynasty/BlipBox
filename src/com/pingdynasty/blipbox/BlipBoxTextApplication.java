package com.pingdynasty.blipbox;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.apache.log4j.Logger;

public class BlipBoxTextApplication extends BlipBoxApplication {
    private static final Logger log = Logger.getLogger(BlipBoxTextApplication.class);
    private TextProcessingEventHandler eventhandler;

    public BlipBoxTextApplication(String config){
        super(config);
        eventhandler = new TextProcessingEventHandler(sender);
        receiver.setSensorEventHandler(eventhandler);
        TextProcessingCanvas canvas = new TextProcessingCanvas(eventhandler, sender);
        JFrame frame = createFrame();
        frame.add(canvas);
        frame.setVisible(true);
    }

    public void start(){
        sender.setFollowMode("None");
        log.info("Blipbox text application started");
    }
}
