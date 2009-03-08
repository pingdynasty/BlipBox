package com.pingdynasty.blipbox;

import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.swing.JFrame;
import org.apache.log4j.Logger;

public class CommandLine {
    private static final Logger log = Logger.getLogger(CommandLine.class);

    public static void main(String[] args)
        throws Exception {

        String serialport = "/dev/tty.usbserial-A6004bII";
        int serialspeed = 115200;
        OutputStream logStream = null;

        boolean graph = false;
        boolean midi = false;
        boolean monome = false;
        String config = "blipbox";

        for(int i=0; i<args.length; ++i){
            if(args[i].equals("-h")){
                log.info("usage: \t-l\t\tlist ports\n"+
                         "\t-g\t\tgraph output\n"+
                         "\t-m\t\tmidi output\n"+
                         "\t-t\t\ttaquito configuration (default: blipbox)\n"+
                         "\t-o file\t\tlog serial output to file\n"+
                         "\t-p port\t\tspecify port\n"+
                         "\t-b rate\t\tspecify baud rate (default 115200)\n"+
                         "\t--monome\treset device into monome mode\n"+
                         "\t-h\t\tprint help info (this message)");
                return;
            }else if(args[i].equals("--monome")){
                monome = true;
            }else if(args[i].equals("-l")){
                SerialDataHandler.listports();
                return;
            }else if(args[i].equals("-g")){
                graph = true;
            }else if(args[i].equals("-m")){
                midi = true;
            }else if(args[i].equals("-t")){
                config = "taquito";
            }else if(args[i].equals("-p") && ++i < args.length){
                serialport = args[i];
            }else if(args[i].equals("-o") && ++i < args.length){
                logStream = new FileOutputStream(args[i]);
            }else if(args[i].equals("-b") && ++i < args.length){
                serialspeed = Integer.parseInt(args[i]);
            }
        }
        if(serialport == null){
            SerialDataHandler.listports();
            System.err.println("Please specify which port to connect to");
            return;
        }

        BlipBoxDataHandler service = new BlipBoxDataHandler(config);
        if(logStream != null)
            service.setLogStream(logStream);

        try{
            service.openSerialPort(serialport, serialspeed);
        }catch(Exception exc){
            log.error("Failed to open serial port "+serialport, exc);
        }
        Thread.sleep(100); // wait for serial line to settle / device to reset

        MidiOutputEventHandler midihandler = null;

        if(monome){
            log.debug("Resetting into monome mode");
            Thread.sleep(2000);
            service.getSerialPort().setDTR(true);
            // diagnostics...
            for(int i=0; i<80; i+=2)
                service.setLed(i, i % 4 == 0 ? 255 : 127);
            Thread.sleep(6000);
            // todo: this doesn't work
//             service.reset(BlipBoxDataHandler.BlipBoxMode.MONOME_MODE);
            service.getSerialPort().setDTR(false);
            Thread.sleep(6000);
            return;
        }
        if(graph)
            service.createGraph();

        if(midi)
            midihandler = service.createMidiOutput();

        if(midihandler != null){
            MidiConfigurationCanvas canvas = new MidiConfigurationCanvas(midihandler, service);
            JFrame frame = new JFrame();
            frame.add(canvas);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(800, 400);
            frame.setVisible(true);
        }

//         service.initialiseBlipBox();

        for(;;);
    }

}