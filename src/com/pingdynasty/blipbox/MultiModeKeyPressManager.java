package com.pingdynasty.blipbox;

import java.util.Set;
import java.util.Map;
import java.util.HashMap;
import javax.sound.midi.*;
import com.pingdynasty.midi.ScaleMapper;
import org.apache.log4j.Logger;

public class MultiModeKeyPressManager extends KeyPressManager {
    private static final Logger log = Logger.getLogger(MultiModeKeyPressManager.class);

    private Map<String, ConfigurationMode> configurations = 
               new HashMap<String, ConfigurationMode>();
    private boolean released; // flag to say if keys have been released since last mode switch
    private ConfigurationMode currentConfig;
    private BlipBox sender;
    private int holdBrightness = 0x80;

    public class ModeChangeEventHandler implements SensorEventHandler {
        private String mode;
        public ModeChangeEventHandler(String mode){
            this.mode = mode;
        }
        public void sensorChange(SensorDefinition sensor){
            if(sensor.value != 0)
                setMode(mode);
            sender.setFollowMode(currentConfig.getFollowMode());
            if(isPressed()){
                holdOn();
            }else{
                holdOff();
            }
        }
    }

    public MultiModeKeyPressManager(BlipBox sender){
        this.sender = sender;
    }

    public String[] getModeNames(){
        Set<String> names = configurations.keySet();
        String[] array = new String[names.size()];
        names.toArray(array);
        return array;
    }

    public void holdOn(){
        // inverting and adjusting row value...
        sender.setLed(getNumberOfRows()-getRow()-1, getColumn(), holdBrightness);
    }

    public void holdOff(){
        sender.clear();
    }

    public void init(){
        // set up a two mode configuration
        setConfigurationMode(createConfigurationMode("Cross", "Cross"));
        setConfigurationMode(createConfigurationMode("Criss", "Criss"));
        setMode("Cross");
    }

    public void setConfigurationMode(ConfigurationMode mode){
        configurations.put(mode.getName(), mode);
    }

    public ConfigurationMode createConfigurationMode(String mode, String follow){
        return new ConfigurationMode(mode, follow);
    }

    public void setKeyEventHandler(String mode, KeyEventHandler handler){
        getSensorHandlerConfiguration(mode).registerKeyEventHandler(handler);
    }

    public void setSensorEventHandler(String mode, SensorType type, SensorEventHandler handler){
        getSensorHandlerConfiguration(mode).registerSensorEventHandler(type, handler);
    }

    public void configureUnassigned(String mode, SensorType type){
        log.debug("Setting "+mode+":"+type+" to unassigned");
        setSensorEventHandler(mode, type, null);
    }

    public void configureModeChange(String mode, SensorType type, String toMode){
        log.debug("Setting "+mode+":"+type+" to mode change");
        setSensorEventHandler(mode, type, new ModeChangeEventHandler(toMode));
    }

    public SensorHandlerConfiguration getSensorHandlerConfiguration(String mode){
        return getConfigurationMode(mode).getSensorHandlerConfiguration();
    }

    public ConfigurationMode getCurrentConfigurationMode(){
        return currentConfig;
    }

    public ConfigurationMode getConfigurationMode(String mode){
         ConfigurationMode config = configurations.get(mode);
        if(config == null)
            throw new IllegalArgumentException("No such mode: "+mode);
        return config;
    }

    public void setMode(String mode){
        log.debug("set mode: "+mode);
        currentConfig = getConfigurationMode(mode);
    }

    // event handlers

    public void sensorChange(SensorDefinition sensor){
        currentConfig.getSensorHandlerConfiguration().sensorChange(sensor);
        super.sensorChange(sensor); // trigger key events
    }

    public void keyDown(int col, int row){
        released = true;
        currentConfig.getSensorHandlerConfiguration().keyDown(col, row);
    }

    public void keyUp(int col, int row){
        released = true;
        currentConfig.getSensorHandlerConfiguration().keyUp(col, row);
    }

    public void keyChange(int oldCol, int oldRow, int newCol, int newRow){
        if(released){
            /// suppress keychange event if key has not been released since last mode switch (key hold)
            currentConfig.getSensorHandlerConfiguration().keyChange(oldCol, oldRow, newCol, newRow);
        }
    }
}