// comment out #include "wiring.h" in MessageSender.cpp then make and run with
// g++ MessageSenderTest.cpp -o tester && ./tester | hexdump

#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

void beginSerial(long baud){}

uint16_t getParameter(uint8_t pid){
  return 0xf0;
}

void serialWrite(unsigned char c){
//    printf("value: 0x%x / %d / %c\n", c, c, c);
  putchar(c);
}

#define _BV(bit) (1 << (bit))

#define SENSOR_MAX 1023

#define MESSAGE_SENDER_TEST
#include "MessageSender.cpp"

MessageSender sender;

void interval(){
  for(int i=0; i<7; ++i)
    sender.sendNextMessage();
//   printf("sleeping 1 sec\n");
//   sleep(1);
}

int main(int argc, char* argv[]) {

  sender.init();

  for(int i=1; i<argc; ++i){
    printf("sending %c", argv[i][0]);
    switch(argv[i][0]){
    case '1':
      sender.updateRelease();
      break;
    case '2':
      sender.updateXY(argv[i][1], argv[i][2], SENSOR_MAX);
      break;
    case '3':
      sender.updateSensor(BUTTON1_SENSOR, argv[i][1]);
      break;
    case 's':
      sender.sendNextMessage();
      break;
    }
  }

//   sender.updateRelease();
//   sender.sendNextMessage();
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   sender.sendNextMessage();
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0);
//   sender.sendNextMessage();
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//     sender.sendNextMessage();
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0);
//     sender.sendNextMessage();

//   sender.updateSensor(POT_SENSOR, 123);
//   interval();

//   sender.updateSensor(POT_SENSOR, 234);
//   sender.updateSensor(POT_SENSOR, 12);
//   sender.updateSensor(POT_SENSOR, 43);
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

//   sender.updateSensor(POT_SENSOR, 432);
//   sender.updateSensor(POT_SENSOR, 123);
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   interval();

//   sender.updateXY(0, 0, SENSOR_MAX);

//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   sender.updateSensor(BUTTON1_SENSOR, 0x00);

//   sender.updateRelease();
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

//   sender.updateXY(SENSOR_MAX, SENSOR_MAX, 100);

//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   interval();

//   sender.updateRelease();
//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   interval();


//   sender.updateXY(SENSOR_MAX, SENSOR_MAX, 123);
//   interval();

//   sender.updateXY(0, 0, 145);
//   interval();

//   sender.updateXY(123, 234, 32);
//   interval();

//   sender.updateXY(0, SENSOR_MAX, 43);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0x00);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, 0x01);
//   interval();

//   sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
//   interval();

}


