#include <stdio.h>

void beginSerial(long baud)
{
  printf("beginSerial %ld\n", baud);
}

void serialWrite(unsigned char c)
{
  printf("tx 0x%x\n", c);
}
