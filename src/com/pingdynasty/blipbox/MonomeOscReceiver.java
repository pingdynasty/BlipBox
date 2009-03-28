package com.pingdynasty.blipbox;

import java.util.Vector;
import org.apache.log4j.Logger;
import se.antimon.osc.OscReceiver;

public class MonomeOscReceiver extends OscReceiver {
    private MonomeInput monome;
    private String prefix;
    private String ledCommand;
    private String ledRowCommand;
    private String ledColCommand;
    private String frameCommand;
    private String clearCommand;
    private String prefixCommand;
    private String testCommand;
    private String offsetCommand;
    private String reportCommand;

    public MonomeOscReceiver(MonomeInput monome){
        this.monome = monome;
        setPrefix(monome.getPrefix());
    }

    public void setPrefix(String prefix){
        this.prefix = prefix;
        ledCommand = prefix+"/led";
        ledRowCommand = prefix+"/led_row";
        ledColCommand = prefix+"/led_col";
        frameCommand = prefix+"/frame";
        clearCommand = prefix+"/clear";
        prefixCommand = "/sys/prefix";
        testCommand = "/sys/test";
        offsetCommand = "/sys/offset";
        reportCommand = "/sys/report";
    }
    
    protected void receiveCommand(String command, Vector<Object> arguments) {
        if(command.equals(ledCommand)){
            int x = ((Integer)arguments.get(0)).intValue();
            int y = ((Integer)arguments.get(1)).intValue();
            boolean state = ((Integer)arguments.get(2)).intValue() == 1;
            System.out.println(ledCommand+" "+x+" "+y+" "+state);
            monome.led(x, y, state);
        }else if(command.equals(ledRowCommand)){
            int row = ((Integer)arguments.get(0)).intValue();
            int data = ((Integer)arguments.get(1)).intValue();
            System.out.println(ledRowCommand+" "+row+" "+data);
            monome.led_row(row, data);
        }else if(command.equals(ledColCommand)){
            int col = ((Integer)arguments.get(0)).intValue();
            int data = ((Integer)arguments.get(1)).intValue();
            System.out.println(ledColCommand+" "+col+" "+data);
            monome.led_col(col, data);
        }else if(command.equals(frameCommand) && arguments.size() == 8){
            int[] data = new int[8];
            System.out.print(frameCommand);
            for(int i=0; i<8; ++i){
                data[i] = ((Integer)arguments.get(0)).intValue();
                System.out.print(" "+data[i]);
            }
            System.out.println();
            monome.frame(data);
        }else if(command.equals(clearCommand)){
            boolean state = arguments.size() > 0 && ((Integer)arguments.get(0)).intValue() == 1;
            System.out.println(clearCommand+" "+state);
            monome.clear(state);
        }else if(command.equals(prefixCommand)){
            setPrefix((String)arguments.get(0));
            System.out.println(prefixCommand+" "+prefix);
            monome.prefix(prefix);
        }else if(command.equals(testCommand)){
            boolean state = arguments.size() == 1 && ((Integer)arguments.get(0)).intValue() == 1
                || arguments.size() == 2 && ((Integer)arguments.get(1)).intValue() == 1;
            monome.test(state);
        }else{
            System.out.println("Unmatched "+command+" ["+arguments.size()+"]");
        }
    }
}
