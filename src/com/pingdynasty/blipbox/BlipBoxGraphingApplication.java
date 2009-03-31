package com.pingdynasty.blipbox;

import org.apache.log4j.Logger;
import javax.swing.JFrame;

public class BlipBoxGraphingApplication extends BlipBoxApplication {
    private static final Logger log = Logger.getLogger(BlipBoxGraphingApplication.class);
    private GraphingEventHandler eventhandler;

    public BlipBoxGraphingApplication(String config){
        super(config);
        eventhandler = new GraphingEventHandler();

        JFrame frame = createFrame();
        frame.add(eventhandler.getCanvas());
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(860, 1024);
        frame.setVisible(true);

        receiver.setSensorEventHandler(eventhandler);
    }

}
