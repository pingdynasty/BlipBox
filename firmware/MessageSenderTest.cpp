// comment out #include "wiring.h" in MessageSender.cpp then make and run with
// g++ MessageSenderTest.cpp -o test && ./test | hexdump

#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

void beginSerial(long baud){}

void serialWrite(unsigned char c){
//    printf("value: 0x%x / %d / %c\n", c, c, c);
  putchar(c);
}

#define _BV(bit) (1 << (bit))

#define MESSAGE_SENDER_TEST
#include "MessageSender.cpp"

MessageSender sender;

void interval(){
  for(int i=0; i<7; ++i)
    sender.sendNextMessage();
//   printf("sleeping 1 sec\n");
//   sleep(1);
}

int main(void)
{
  sender.init();

  sender.updateRelease();
    sender.sendNextMessage();
  sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
    sender.sendNextMessage();
  interval();

  sender.updateSensor(BUTTON1_SENSOR, 0);
    sender.sendNextMessage();
  interval();

  sender.updateSensor(BUTTON1_SENSOR, SENSOR_MAX);
    sender.sendNextMessage();
  interval();

  sender.updateSensor(BUTTON1_SENSOR, 0);
    sender.sendNextMessage();

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


