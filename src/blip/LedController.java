package blip;

public class LedController {

//     public void setCross(int x, int y, int value){
//         byte[] msg = new byte[36];
//         for(int i=0; i<10; ++i){
//             msg[2*i] = (byte)(i*16+y);
//             msg[2*i+1] = (byte)(value / (4 * Math.abs(x-i) + 1));
//         }
//         for(int i=0; i<8; ++i){
//             msg[2*i+20] = (byte)(x*16+i);
//             msg[2*i+21] = (byte)(value / (4 * Math.abs(y-i) + 1));
//         }
//         try{
//             out.write(msg);
//         }catch(Exception exc){
//             exc.printStackTrace();
//         }
//     }


//     public void setCross(int x, int y, int value){
//                 for(int i=0; i<10; ++i)
//             setLed(i, y, value / (Math.abs(x-i) + 1));
//         for(int i=0; i<8; ++i)
//             setLed(x, i, value / (Math.abs(y-i) + 1));
//     }

}