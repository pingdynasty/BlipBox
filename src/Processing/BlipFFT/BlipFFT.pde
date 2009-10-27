/**
 * By Evan Raskob 2009, modified from 
 * Forward FFT by Damien Di Fede, part of
 * Minim examples for Processing: http://processing.org
 *
 * This sketch demonstrates how to use an FFT to analyze an AudioBuffer 
 * and draw the resulting spectrum. It also manually sums up interesting portions
 * of the spectrum.  
 * Press 'w' to turn on windowing, press 'e' to turn it off.
 *
 * From Openlab Workshops
 * http://openlabworkshops.org
 * http://pixelist.info
 *
 *
 */

import ddf.minim.analysis.*;
import ddf.minim.*;
import javax.sound.sampled.FloatControl;
import javax.sound.sampled.Control;

ProcessingBlipBox blipbox;

Minim minim;
AudioSource player;
FFT fft;
String windowName;

Control[] controls;
AudioOutput out;

AudioSource openFile(){
  AudioPlayer player = minim.loadFile("groove.mp3", 1024);
  player.loop();
  return player;
}

AudioSource openLineIn(){
  AudioSource in = minim.getLineIn(Minim.STEREO, 1024);
  return in;
}

void setup()
{
  blipbox = new ProcessingBlipBox(this, 0);

  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut();

//  player = openFile();
  player = openLineIn();


  // change playback rate:  
  /*
  controls = out.getControls();
   for ( int i = 0; i < controls.length; i++ )
   {
   println("Control " + (i+1) + " is a " + controls[i].toString() + ".");
   }
   FloatControl sr =  (FloatControl) player.getControl(FloatControl.Type.SAMPLE_RATE);
   sr.setValue(player.sampleRate()/2);
   */


  // create an FFT object that has a time-domain buffer the same size as jingle's sample buffer
  // note that this needs to be a power of two and that it means the size of the spectrum
  // will be 512. see the online tutorial for more info.
  fft = new FFT(player.bufferSize(), player.sampleRate()/2);
  textFont(createFont("SanSerif", 12));
  windowName = "None";
}

void draw()
{
  //  blipbox.fade();
  background(0);
  stroke(255);
  // perform a forward FFT on the samples in jingle's left buffer
  // note that if jingle were a MONO file, this would be the same as using jingle.right or jingle.left
  fft.forward(player.mix);

  float bass, midrange, treble;

  bass = midrange = treble = 0.0;

  int range1 = fft.specSize()/8;
  int range2 = 5*fft.specSize()/8;
  int range3 = fft.specSize();

  for(int i = 0; i < range1; i++)
  {
    bass += fft.getBand(i);
  }
  bass /= range1;
  bass *= height/2;


  for(int i = range1; i < range2; i++)
  {
    midrange += fft.getBand(i);
  }
  //take average 
  midrange /= (range2-range1);
  midrange *= height/2;

  for(int i = range2; i < range3; i++)
  {
    treble += fft.getBand(i);
  }  
  treble /= (range3-range2);
  treble *= height/2;

  //bass /= 5;
  //bass /= 2;
  treble *= 30;

  // midrange *= 5;
  // treble *= 25;

  fill(128,0,128);
  beginShape();
  vertex(0,height);
  vertex(0,height-bass);
  vertex(width/6, height-bass);
  vertex(width/6, height-midrange);
  vertex(5*width/8, height-midrange);
  vertex(5*width/8, height-treble);
  vertex(width, height-treble);
  vertex(width, height);
  endShape();

  fill(200,200,200);
  beginShape();
  vertex(0,height);

  for(int i = 0; i < fft.specSize(); i++)
  {
    // draw the line for frequency band i, scaling it by 4 so we can see it a bit better
    vertex(width*pow((float)i/(float)fft.specSize(), 0.5),  height - fft.getBand(i)*6);
  }
  vertex(0, width);
  endShape();

  beginShape(LINES);
  for(int i = 0; i < player.left.size(); i++)
  {
    vertex(width*(float)i/(float)player.left.size(), 
    (player.right.get(i)+1.0)*0.3*height);
  }
  endShape();


  fill(255);
  // keep us informed about the window being used
  //  text("The window being used is: " + windowName, 5, 20);
  blipbox.setLedColumn(1, getBar(bass, 20));
  blipbox.setLedColumn(2, getBar(bass, 20));
  blipbox.setLedColumn(4, getBar(midrange, 20));
  blipbox.setLedColumn(5, getBar(midrange, 20));
  blipbox.setLedColumn(7, getBar(treble, 20));
  blipbox.setLedColumn(8, getBar(treble, 20));
}

int getBar(float value, int scale){
  return getBar((int)value/scale);
}

int getBar(int value){
  int result = 0;
  for(int i=0; i<value; ++i)
    result |= 1 << i;
  return result;
}

void keyReleased()
{
  if ( key == 'w' ) 
  {
    // a Hamming window can be used to shape the sample buffer that is passed to the FFT
    // this can reduce the amount of noise in the spectrum
    fft.window(FFT.HAMMING);
    windowName = "Hamming";
  }

  if ( key == 'e' ) 
  {
    fft.window(FFT.NONE);
    windowName = "None";
  }
}

void stop()
{
  // always close Minim audio classes when you finish with them
  player.close();
  minim.stop();
  blipbox.stop();
  super.stop();
}




