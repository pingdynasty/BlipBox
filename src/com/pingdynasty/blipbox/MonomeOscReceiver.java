package com.pingdynasty.blipbox;

import java.util.Vector;
import org.apache.log4j.Logger;
import se.antimon.osc.OscReceiver;

public class MonomeOscReceiver extends OscReceiver {
    private MonomeInput monome;
    private String ledCommand = "/led";
    private String ledRowCommand = "/led_row";
    private String ledColCommand = "/led_col";
    private String frameCommand = "/frame";
    private String clearCommand = "/clear";
    private String prefixCommand = "/sys/prefix";
    private String testCommand = "/sys/test";
    private String offsetCommand = "/sys/offset";
    private String reportCommand = "/sys/report";

    public MonomeOscReceiver(MonomeInput monome){
        this.monome = monome;
    }
    
    protected void receiveCommand(String command, Vector<Object> arguments) {
        if(command.equals(monome.getPrefix()+ledCommand)){
            int x = ((Integer)arguments.get(0)).intValue();
            int y = ((Integer)arguments.get(1)).intValue();
            boolean state = ((Integer)arguments.get(2)).intValue() == 1;
            System.out.println(ledCommand+" "+x+" "+y+" "+state);
            monome.led(x, y, state);
        }else if(command.equals(monome.getPrefix()+ledRowCommand)){
            int row = ((Integer)arguments.get(0)).intValue();
            int data = ((Integer)arguments.get(1)).intValue();
            System.out.println(ledRowCommand+" "+row+" "+data);
            monome.led_row(row, data);
        }else if(command.equals(monome.getPrefix()+ledColCommand)){
            int col = ((Integer)arguments.get(0)).intValue();
            int data = ((Integer)arguments.get(1)).intValue();
            System.out.println(ledColCommand+" "+col+" "+data);
            monome.led_col(col, data);
        }else if(command.equals(monome.getPrefix()+frameCommand) && arguments.size() == 8){
            int[] data = new int[8];
            System.out.print(frameCommand);
            for(int i=0; i<8; ++i){
                data[i] = ((Integer)arguments.get(0)).intValue();
                System.out.print(" "+data[i]);
            }
            System.out.println();
            monome.frame(data);
        }else if(command.equals(monome.getPrefix()+clearCommand)){
            boolean state = arguments.size() > 0 && ((Integer)arguments.get(0)).intValue() == 1;
            System.out.println(clearCommand+" "+state);
            monome.clear(state);
        }else if(command.equals(prefixCommand)){
            String prefix = (String)arguments.get(0);
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
