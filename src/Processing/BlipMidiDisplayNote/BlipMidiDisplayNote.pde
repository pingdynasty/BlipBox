import rwmidi.*;

rwmidi.MidiInput input;
ProcessingBlipBox blipbox;
com.pingdynasty.midi.NoteParser parser;

int speed = 100;

void setup() {
  input = RWMidi.getInputDevices()[0].createInput(this);
  blipbox = new ProcessingBlipBox(this, 0);
}

public void noteOnReceived(Note note){
  String str = parser.getStringNote(note.getPitch(), false);
  blipbox.writeCharacter(0, str.charAt(1));
  blipbox.writeCharacter(5, str.charAt(0));
//  blipbox.writeText(str, speed);
  println(str+" "+note);
}

void draw() {
  blipbox.fade();
  delay(speed);
}






