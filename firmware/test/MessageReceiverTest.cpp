// g++ MessageReceiverTest.cpp -o test && ./test

#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

#define TEST_DATA_LENGTH 31

uint8_t test_data[TEST_DATA_LENGTH] = {
     0x20, 
     0x40, 0x0, 0x0f, 
     0x40, 0x1, 0xff, 
     0x40, 0x2, 0x0f, 
     0x40, 0x3, 0xff, 
     0x40, 0x4, 0x0f, 
     0x40, 0x5, 0xff, 
     0x40, 0x6, 0x0f, 
     0x40, 0x7, 0xff, 
     0x40, 0x8, 0x0f, 
     0x40, 0x9, 0xff
   };

int pos;
int serialRead(){
  return test_data[pos++];
}

int serialAvailable(){
  return TEST_DATA_LENGTH - pos;
}

void error(uint8_t code){
  printf("error "+code);
}

#define _BV(bit) (1 << (bit))
#define MESSAGE_READ_ERROR 4
#define ROWS 5
#define COLS 16

#define MESSAGE_RECEIVER_TEST
#include "../MessageReceiver.cpp"

MessageReceiver receiver;

uint8_t follow;
uint8_t holding;
uint8_t holdRow;
uint8_t holdCol;

void readMessage(){
  switch(receiver.getMessageType()){
  case CLEAR_MESSAGE:
    printf("clear\n");
    break;
  case FOLLOW_MODE_MESSAGE:
    follow = receiver.getMessageData()[0] & 0x0f;
    printf("follow %d\n", follow);
    break;
  case SET_LED_MESSAGE:
    if(follow){
      holding = receiver.getMessageData()[2];
      holdRow = receiver.getMessageData()[1] / COLS;
      holdCol = receiver.getMessageData()[1] % COLS;
      printf("holding %d %d 0x%x\n", holdCol, holdRow, holding);
    }else{
      uint8_t row = receiver.getMessageData()[1] / COLS;
      uint8_t col = receiver.getMessageData()[1] % COLS;
      uint8_t brt = receiver.getMessageData()[2];
      printf("led %d %d 0x%x\n", col, row, brt);
    }
    break;
  }
}

int main(void)
{
  while(serialAvailable())
    if(receiver.receiveMessage())
      readMessage();
}
