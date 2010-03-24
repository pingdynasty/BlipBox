// #include "spi.h"

#define MIC_OUTPUT_ENABLE 2
#define MIC_STROBE        3
#define MIC_CLOCK         4
#define MIC_DATA          5

/* MIC pins
LOGIC SUPPLY - vcc
LOAD SUPPLY - vcc
GROUND - gnd
OUT1-8 - led - 500R - gnd
*/

void setup() { 
  Serial.begin(9600);
  Serial.println("hello");

//   spi_init();

  pinMode(MIC_OUTPUT_ENABLE, OUTPUT);
  pinMode(MIC_STROBE, OUTPUT);
  pinMode(MIC_CLOCK, OUTPUT);
  pinMode(MIC_DATA, OUTPUT);
 
  Serial.println("let's go");
}

byte read_console_char(){
  while(!Serial.available());
  return Serial.read();
}

void mic_transfer_bit(uint8_t bit){
  digitalWrite(MIC_DATA, bit ? HIGH : LOW);
  digitalWrite(MIC_CLOCK, HIGH);
  digitalWrite(MIC_CLOCK, LOW);
  Serial.print(bit, HEX);
  Serial.println(" transferred");
}

bool strobe;
void mic_toggle_strobe(){
  strobe = !strobe;
  digitalWrite(MIC_STROBE, strobe ? HIGH : LOW);
  Serial.print("strobe ");
  Serial.println(strobe, HEX);
}

bool output;
void mic_toggle_output(){
  output = !output;
  digitalWrite(MIC_OUTPUT_ENABLE, output ? HIGH : LOW);
  Serial.print("output ");
  Serial.println(output, HEX);
}

void loop () {
  switch(read_console_char()){
  case '0':
    mic_transfer_bit(0);
    break;
  case '1':
    mic_transfer_bit(1);
    break;
  case 's':
    mic_toggle_strobe();
    break;
  case 'o':
    mic_toggle_output();
    break;
  }
}
