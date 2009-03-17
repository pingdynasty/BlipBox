package com.pingdynasty.blipbox;

public class ConfigurationMode {
    private String name;
    private String follow;
    private SensorHandlerConfiguration config;

    public ConfigurationMode(String name, String follow){
        this.name = name;
        this.follow = follow;
        config = new SensorHandlerConfiguration();
    }

    public SensorHandlerConfiguration getSensorHandlerConfiguration(){
        return config;
    }

    public String getName(){
        return name;
    }

    public String getFollowMode(){
        return follow;
    }
}

