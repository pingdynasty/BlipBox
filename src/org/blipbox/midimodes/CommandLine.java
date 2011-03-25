package org.blipbox.midimodes;

import org.apache.log4j.Logger;
import org.blipbox.*;

public class CommandLine {
    private static final Logger log = Logger.getLogger(CommandLine.class);

    public static void main(String[] args)
        throws Exception {

        log.info("Configuring application");

        MidiModesBlipBox application = new MidiModesBlipBox();
        String serialport = null;
        int serialspeed = 57600;

        for(int i=0; i<args.length; ++i){
            if(args[i].equals("-h")){
                log.info("usage:\n"+
                         " \t-l\t\tlist ports\n"+
                         "\t-p port\t\tspecify port\n"+
                         "\t-b rate\t\tspecify baud rate (default "+serialspeed+")\n"+
                         "\t-h\t\tprint help info (this message)");
                return;
            }else if(args[i].equals("-l")){
                BlipBoxDataReceiver.listports(System.out);
                return;
            }else if(args[i].equals("-p") && ++i < args.length){
                serialport = args[i];
            }else if(args[i].equals("-b") && ++i < args.length){
                serialspeed = Integer.parseInt(args[i]);
            }
        }

        if(serialport == null){
            BlipBoxDataReceiver.listports(System.out);
            System.err.println("Please specify which port to connect to");
            return;
        }

        application.setSerialPort(serialport);
        application.setSerialSpeed(serialspeed);
        application.openSerialPort();
        Thread.sleep(2000); // wait for serial line to settle / device to reset

        log.info("Starting application");
        application.start();
    }

}