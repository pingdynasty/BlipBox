package com.pingdynasty.blipbox;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.List;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.TooManyListenersException;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;
import org.apache.log4j.Logger;
import javax.swing.JFrame;
import java.awt.Canvas;

public class GraphingDataHandler extends SerialDataHandler {
    private static final Logger log = Logger.getLogger(GraphingDataHandler.class);

    private MultiTouchscreenCanvas canvas;

    public void run(){
        canvas = new MultiTouchscreenCanvas();
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(1024, 1024);
        frame.setTitle("BlipGrapher");
        frame.add(canvas);
        frame.setVisible(true);        
    }

    public void handle(String data){
        String[] parts = data.split(",");
        int[] ints = new int[parts.length];
        for(int i=0; i<ints.length; ++i)
            ints[i] = Integer.parseInt(parts[i]);

//         canvas.update(new int[]{ints[2], ints[5],
//                                 ints[3], ints[6],
//                                 ints[4], ints[7]});

//         canvas.update(new int[]{ints[2], ints[6],
//                                 ints[3], ints[6],
//                                 ints[4], ints[6]});

//         canvas.update(new int[]{ints[2], ints[6],
//                                 ints[3], ints[6],
//                                 ints[4], ints[6],
//                                 ints[3], ints[5],
//                                 ints[3], ints[7]});


//         canvas.update(new int[]{ints[2], ints[5],
//                                 ints[2], ints[7],
//                                 ints[3], ints[6],
//                                 ints[4], ints[5],
//                                 ints[4], ints[7],
//                                 ints[8], ints[9]});

        int x1 = ints[2];
        int x2 = ints[3];
        int x3 = ints[4];
        int y1 = ints[5];
        int y2 = ints[6];
        int y3 = ints[7];
        canvas.update(new int[]{ x2, y2,
                                 x1(x2, y2), y2,
                                 x1, y2,
                                 x3(x2, y2), y2,
                                 x3, y2
            });
    }

    private int y1(int x2, int y2) { 
        return 4561/9 - 83*x2/318 + 74141*y2/190800;
    }

    private int y3(int x2, int y2) { 
        return -1117/9 + 43*x2/159 + 81353*y2/95400;
    }

    private int x1(int x2, int y2) { 
        return 4958/9 + 46*x2/159 - 473*y2/1908;
    }

    private int x3(int x2, int y2) { 
        return -1297/9 + 281*x2/318 + 48901*y2/190800;
    }


    public static void main(String[] args)
        throws Exception {
        String serialport = null;
        int serialspeed = 9600;
        GraphingDataHandler service = new GraphingDataHandler();

        for(int i=0; i<args.length; ++i){
            if(args[i].equals("-h")){
                log.info("usage: \t-l\t\tlist ports\n"+
                         "\t-o file\t\tlog serial output to file\n"+
                         "\t-p port\t\tspecify port\n"+
                         "\t-b rate\t\tspecify baud rate (default 115200)\n"+
                         "\t-h\t\tprint help info (this message)");
                return;
            }else if(args[i].equals("-l")){
                listports(System.out);
                return;
            }else if(args[i].equals("-p") && ++i < args.length){
                serialport = args[i];
            }else if(args[i].equals("-o") && ++i < args.length){
                service.setLogStream(new FileOutputStream(args[i]));
            }else if(args[i].equals("-b") && ++i < args.length){
                serialspeed = Integer.parseInt(args[i]);
            }else if(args[i].equals("-flow") && ++i < args.length){
                service.setFlowControlMode(args[i]);
            }
        }
        if(serialport == null){
            listports(System.err);
            System.err.println("Please specify which port to connect to");
            return;
        }

        try{
            service.openSerialPort(serialport, serialspeed);
        }catch(Exception exc){
            log.error("Failed to open serial port "+serialport, exc);
            return;
        }

        service.run();
    }
}