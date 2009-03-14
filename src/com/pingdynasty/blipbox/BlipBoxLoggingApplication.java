package com.pingdynasty.blipbox;

import org.apache.log4j.Logger;

public class BlipBoxLoggingApplication extends BlipBoxApplication {
    private static final Logger log = Logger.getLogger(BlipBoxLoggingApplication.class);
    private LoggingEventHandler eventhandler;

    public BlipBoxLoggingApplication(String config){
        super(config);
        eventhandler = new LoggingEventHandler();
        receiver.setSensorEventHandler(eventhandler);
    }

}
