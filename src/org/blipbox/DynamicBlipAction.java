package org.blipbox;

import java.lang.reflect.Method;

public class DynamicBlipAction extends AbstractBlipAction {
    private Object obj;
    private Method press;
    private Method drag;
    private Method release;
    private Method tap;
    private Method taptap;
    private Method position;
    private Method parameter;

    public DynamicBlipAction(Object obj){
        this.obj = obj;
        press = getMethod("touchPressed", new Class[]{Position.class});
        drag = getMethod("touchDragged", new Class[]{Position.class, Position.class});
        release = getMethod("touchReleased", new Class[]{Position.class});
        tap = getMethod("touchClicked", new Class[]{Position.class});
        taptap = getMethod("touchDoubleClicked", new Class[]{Position.class});
        position = getMethod("touch", new Class[]{Position.class});
        parameter = getMethod("parameter", new Class[]{Parameter.class, Integer.TYPE});
    }

    private Method getMethod(String name, Class[] sig){
        try{
            return obj.getClass().getMethod(name, sig);
        }catch(NoSuchMethodException exc){
            return null;
        }
    }

    public void press(Position pos){
        if(press != null)
            try{
                press.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void drag(Position from, Position to){
        if(drag != null)
            try{
                drag.invoke(obj, new Object[]{from, to});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void release(Position pos){
        if(release != null)
            try{
                release.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void tap(Position pos){
        if(tap != null)
            try{
                tap.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void taptap(Position pos){
        if(taptap != null)
            try{
                taptap.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void position(Position pos){
        if(position != null)
            try{
                position.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void parameterValue(Parameter param, int value){
	if(parameter != null){
            try{
                parameter.invoke(obj, new Object[]{param, value});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
	}
    }

}
