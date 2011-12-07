#include "serial.h"
#include "MidiWriter.h"

void MidiWriter::write(uint8_t data){
  serialWrite(data);
//     Serial.print(data);
}
