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
        tap = getMethod("tap", new Class[]{Integer.TYPE, Integer.TYPE});
        taptap = getMethod("taptap", new Class[]{Integer.TYPE, Integer.TYPE});
        release = getMethod("release", new Class[]{Integer.TYPE, Integer.TYPE});
        drag = getMethod("drag", new Class[]{Integer.TYPE, Integer.TYPE, Integer.TYPE, Integer.TYPE});
        position = getMethod("position", new Class[]{Integer.TYPE, Integer.TYPE});
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

    public void tap(int x, int y){
        if(tap != null)
            try{
                tap.invoke(obj, new Object[]{x, y});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void taptap(int x, int y){
        if(taptap != null)
            try{
                taptap.invoke(obj, new Object[]{x, y});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void release(int x, int y){
        if(release != null)
            try{
                release.invoke(obj, new Object[]{x, y});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void drag(int x, int y, int dx, int dy){
        if(drag != null)
            try{
                drag.invoke(obj, new Object[]{x, y, dx, dy});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

    public void position(int x, int y){
        if(position != null)
            try{
                position.invoke(obj, new Object[]{x, y});
            }catch(Exception exc){
                throw new RuntimeException(exc);
            }
    }

}
