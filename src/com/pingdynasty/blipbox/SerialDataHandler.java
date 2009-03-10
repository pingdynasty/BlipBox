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
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;
import org.apache.log4j.Logger;

public class SerialDataHandler implements SerialPortEventListener {
    private static final Logger log = Logger.getLogger(SerialDataHandler.class);

 SerialPort serialport;
    protected InputStream inStream;
    protected OutputStream outStream;
    protected OutputStream logStream;
    private String str = EMPTY;
    private static final String EMPTY = "";
    private String port;
    private int speed;

    public SerialDataHandler(){
        // empty!
    }

    public int getSerialSpeed(){
        return speed;
    }

    public String getSerialPortName(){
        return port;
    }

    public SerialPort getSerialPort(){
        return serialport;
    }

    public void setLogStream(OutputStream logStream){
        this.logStream = logStream;
    }

    public void handle(String data){
        // overload this method to handle incoming data
        log.info("Received: "+data);
    }

    public void send(int data[]){
        try{
            for(int i : data)
                outStream.write(i);
            outStream.flush();
        }catch(Throwable exc){
            System.err.println("serial io error sending "+data+": "+exc);
        }
    }

    public void send(int data){
        try{
            outStream.write(data);
            outStream.flush();
        }catch(Throwable exc){
//             exc.printStackTrace();
            System.err.println("serial io error sending "+data+": "+exc);
        }
    }

    protected int readSerial()
        throws IOException {
        int c = inStream.read();
        if(logStream != null)
            logStream.write(c);
        return c;
    }

    public void serialEvent(SerialPortEvent event) {
        // note: gripe! no error message if a runtime exception is thrown from here
        if(event.getEventType()== SerialPortEvent.DATA_AVAILABLE) {
            try{
                while(inStream.available() > 0) {
                    char c = (char)readSerial();
                    if(c == '\n' || c == '\r') {
                        if(str != EMPTY)
                            handle(str);
                        str = EMPTY;
                    }else{
                        str += String.valueOf(c);
                    }
                }
                if(logStream != null)
                    logStream.flush();
            }catch(Throwable exc){
                exc.printStackTrace();
                System.err.println("serial io error: "+exc);
            }
        }
    }
	
    public static List<String> getSerialPorts() {
        List<String> vp = new ArrayList<String>();
        Enumeration portList = CommPortIdentifier.getPortIdentifiers();
        while (portList.hasMoreElements()) {
            CommPortIdentifier portId = (CommPortIdentifier)portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL){
                vp.add(portId.getName());
            }
        }
        return vp;
    }

    public static void listports(PrintStream stream){
        List<String> ports = SerialDataHandler.getSerialPorts();
        stream.println("available serial ports: ");
        for(int i=0; i<ports.size(); ++i)
            stream.println(ports.get(i));
    }
	
    public void closeSerialPort() {
        if(serialport == null)
            return;
        log.info("Closing serial port "+serialport.getName());
        serialport.close();
    }

    public void openSerialPort(String port, int speed) {
        this.port = port;
        this.speed = speed;

        closeSerialPort();

        log.info("Opening serial port: "+port+" at "+speed+" baud");

        //which port you want to use and the baud come in as parameters
        try {
            //find the port
            CommPortIdentifier portId = CommPortIdentifier.getPortIdentifier(port);
            //open the port
            serialport = (SerialPort)portId.open("my_java_serial" + port, 2000);
            //configure the port
            try {
                serialport.setSerialPortParams(speed,
                                            serialport.DATABITS_8,
                                            serialport.STOPBITS_1,
                                            serialport.PARITY_NONE);
//                 // hardware flow control
//                 serialport.setFlowControlMode(serialport.FLOWCONTROL_RTSCTS_OUT);
            } catch (UnsupportedCommOperationException e){
                throw new RuntimeException("Probably an unsupported baud rate", e);
            }
		
            //establish streams for reading and writing to the port
            try {
                inStream = serialport.getInputStream();
                outStream = serialport.getOutputStream();
            } catch (IOException e) {
                throw new RuntimeException("couldn't get streams", e);
            }

            // we could read from "in" in a separate thread, but the API gives us events
            try {
                serialport.addEventListener(this);
                serialport.notifyOnDataAvailable(true);
            } catch (TooManyListenersException e ) {
                throw new RuntimeException("couldn't add listener", e);
            }
		
        } catch (Exception e) { 
            throw new RuntimeException("Port in Use", e);
        }
    }

    public void destroy(){
        try{
            if(logStream != null)
                logStream.close();
        }catch(Exception e){}
        try{
            if(inStream != null)
                inStream.close();
        }catch(Exception e){}
        try{
            if(outStream != null)
                outStream.close();
        }catch(Exception e){}
    }

    public static void listports(){
        List<String> ports = SerialDataHandler.getSerialPorts();
        log.info("available serial ports: ");
        for(String port: ports)
            log.info(port);
    }

    public static void main(String[] args)
        throws Exception {
        String serialport = null;
        int serialspeed = 115200;
        OutputStream logStream = null;
        File file = null;

        for(int i=0; i<args.length; ++i){
            if(args[i].equals("-h")){
                log.info("usage: \t-l\t\tlist ports\n"+
                         "\t-o file\t\tlog serial output to file\n"+
                         "\t-p port\t\tspecify port\n"+
                         "\t-b rate\t\tspecify baud rate (default 115200)\n"+
                         "\t-s FILE\t\tsend FILE across the connection\n"+
                         "\t-h\t\tprint help info (this message)");
                return;
            }else if(args[i].equals("-l")){
                listports();
                return;
            }else if(args[i].equals("-p") && ++i < args.length){
                serialport = args[i];
            }else if(args[i].equals("-o") && ++i < args.length){
                logStream = new FileOutputStream(args[i]);
            }else if(args[i].equals("-b") && ++i < args.length){
                serialspeed = Integer.parseInt(args[i]);
            }else if(args[i].equals("-s") && ++i < args.length){
                file = new File(args[i]);
            }
        }
        if(serialport == null){
            listports();
            System.err.println("Please specify which port to connect to");
            return;
        }

        SerialDataHandler service = new SerialDataHandler();

        if(logStream != null)
            service.setLogStream(logStream);

        try{
            service.openSerialPort(serialport, serialspeed);
        }catch(Exception exc){
            log.error("Failed to open serial port "+serialport, exc);
            return;
        }

        if(file != null){
            try{
//         Thread.sleep(100); // wait for serial line to settle / device to reset
                Thread.sleep(4000);
                System.out.println("Sending file "+file);
                InputStream in = new FileInputStream(file);
                for(int ch = in.read(); ch != -1; ch = in.read()){
                    service.send(ch);
                    Thread.sleep(800);
                    System.out.print('.');
                    System.out.flush();
                }
                System.out.println(" done!");
            }catch(Exception exc){
                log.error("Failed to send file "+file, exc);
            }
        }else{
            for(;;);
        }
//         Thread.sleep(4000);
//         service.closeSerialPort();
    }
}