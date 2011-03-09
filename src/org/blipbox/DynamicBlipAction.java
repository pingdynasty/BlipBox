package org.blipbox;

import java.lang.reflect.Method;

public class DynamicBlipAction extends AbstractBlipAction {
    private Object obj;
    private Method tap;
    private Method taptap;
    private Method release;
    private Method drag;
    private Method position;

    public DynamicBlipAction(Object obj){
        this.obj = obj;
        tap = getMethod("touchPressed", new Class[]{Position.class});
        taptap = getMethod("taptap", new Class[]{Position.class});
        release = getMethod("touchReleased", new Class[]{Position.class});
        drag = getMethod("touchDragged", new Class[]{Position.class, Position.class});
        position = getMethod("touchDragged", new Class[]{Position.class});
    }

    private Method getMethod(String name, Class[] sig){
        try{
            return obj.getClass().getMethod(name, sig);
        }catch(NoSuchMethodException exc){
            return null;
        }
    }

    // todo?
    //     public void parameterValue(int pid, int value){}

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

    public void release(Position pos){
        if(release != null)
            try{
                release.invoke(obj, new Object[]{pos});
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

    public void position(Position pos){
        if(position != null)
            try{
                position.invoke(obj, new Object[]{pos});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

}
