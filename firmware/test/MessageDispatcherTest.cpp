// comment out #include "wiring.h" in MessageSender.cpp then make and run with
// g++ MessageDispatcherTest.cpp -o tester && ./tester | hexdump

#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

void beginSerial(long baud){}

uint16_t getParameter(uint8_t pid){
  return 0xf0;
}

void serialWrite(unsigned char c){
   printf("value: 0x%x / %d\n", c, c, c);
//   putchar(c);
}

#define _BV(bit) (1 << (bit))

#define SENSOR_MAX 1023

#define MESSAGE_SENDER_TEST
#include "MessageDispatcher.h"

MessageDispatcher sender;

void interval(){
  for(int i=0; i<7; ++i)
    sender.send();
//   printf("sleeping 1 sec\n");
//   sleep(1);
}

int main(int argc, char* argv[]) {
  sender.init();
  for(int i=1; i<argc; ++i){
    printf("sending %c\n", argv[i][0]);
    switch(argv[i][0]){
    case '1':
      sender.release.update();
      break;
    case '2':
      sender.position.update(argv[i][1], argv[i][2]);
      break;
    case '3':
      sender.button1.update(argv[i][1]);
      break;
    case '5':
      sender.parameter.update(argv[i][1], argv[i][2]);
      sender.parameter.send();
      break;
    case 's':
      sender.send();
      break;
    }
  }
}


