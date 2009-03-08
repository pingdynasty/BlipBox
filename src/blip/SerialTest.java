package blip;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.PrintStream;
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

public class SerialTest implements SerialPortEventListener {
    private static final Logger log = Logger.getLogger(SerialTest.class);

    private SerialPort serialport;
    protected InputStream inStream;
    protected OutputStream outStream;

    private List<LogEvent> events = new ArrayList<LogEvent>();

    class LogEvent {
        long millis;
        long data;

        public LogEvent(long data){
            millis = System.currentTimeMillis();
            this.data = data;
        }

        public void dump(PrintStream out){
            out.print(millis);
            out.print(',');
            out.println(data);
        }
    }

    int length;
    int delay;

    // volatile?
    int previous;
    volatile int current;
    volatile long start;

    public void dump(PrintStream out){
        for(LogEvent event: events)
            event.dump(out);
    }

    public boolean completed(){
        return current == length;
    }

    public void runTest()
        throws IOException, InterruptedException {
        previous = 0;
        current = 0;
        start = System.currentTimeMillis();
        for(int i=1; i<=length; ++i){
            outStream.write(i);
            if(delay > 0)
                Thread.sleep(delay);
        }
        log.info(length+" bytes transmitted in "+(System.currentTimeMillis() - start)+" ms");
    }

    public void dataAvailable(SerialPortEvent event)
        throws IOException {
        current = inStream.read();
        if(current == length){
            events.add(new LogEvent(start));
            log.info(length+" bytes received in "+(System.currentTimeMillis() - start)+" ms");
        }else if(current != previous+1){
            log.info("got "+current+" expected "+(previous+1));
            events.add(new LogEvent(-1));
        }else{
            previous = current;
        }
    }

    public void serialEvent(SerialPortEvent event) {
        try{
            // note: gripe! no error message if a runtime exception is thrown from here
            if(event.getEventType()== SerialPortEvent.DATA_AVAILABLE)
                dataAvailable(event);
            else if(event.getEventType()== SerialPortEvent.CTS)
                log.info("clear to send");
        }catch(Throwable exc){
            exc.printStackTrace();
            log.error("serial io error", exc);
        }
    }

    public void openSerialPort(String port, int speed) {
        if(serialport != null)
            serialport.close();
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

    public void close(){
        try{
            if(inStream != null)
                inStream.close();
        }catch(Exception exc){exc.printStackTrace();}
        try{
            if(outStream != null)
                outStream.close();
        }catch(Exception exc){exc.printStackTrace();}
        serialport.close();
    }

    public static void main(String[] args)
        throws Exception {

        String serialport = "/dev/tty.usbserial-A6004bII";
        int serialspeed = 115200;

        SerialTest service = new SerialTest();

        PrintStream output = System.out;

        int iterations = 1;
        int length = 16;
        int delay = 20;

        for(int i=0; i<args.length; ++i){
            if(args[i].equals("-h")){
                log.info("usage: \t-l\t\tlist ports\n"+
                         "\t-o file\t\tlog test results to file\n"+
                         "\t-p port\t\tspecify port\n"+
                         "\t-b rate\t\tspecify baud rate (default 115200)\n"+
                         "\t-h\t\tprint help info (this message)");
                return;
            }else if(args[i].equals("-p") && ++i < args.length){
                serialport = args[i];
            }else if(args[i].equals("-b") && ++i < args.length){
                serialspeed = Integer.parseInt(args[i]);
            }else if(args[i].equals("-o") && ++i < args.length){
                output = new PrintStream(new FileOutputStream(args[i]));
            }else if(args[i].equals("-i") && ++i < args.length){
                iterations = Integer.parseInt(args[i]);
            }else if(args[i].equals("-l") && ++i < args.length){
                length = Integer.parseInt(args[i]);
            }else if(args[i].equals("-d") && ++i < args.length){
                delay = Integer.parseInt(args[i]);
            }
        }

        log.info("opening serial port: "+serialport+" at "+serialspeed+" baud");
        service.openSerialPort(serialport, serialspeed);

        // otherwise the initial DTR might still have the device in a reset
        Thread.sleep(100);

        service.length = length;
        service.delay = delay;
        for(int i=0; i<iterations; ++i){
            service.runTest();
            // wait for test to complete
            while(!service.completed())
                Thread.sleep(100);
        }

        service.dump(output);

        service.close();
    }

}