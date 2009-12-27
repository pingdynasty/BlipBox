/* 
   Sine wave table generator
   gcc SineWaveTableGenerator.c -o sines
*/

#include <math.h>
#include <stdio.h>


int size = 256;
int precision = 8;
int offset;
const double PI = 3.141592653;

long sine(int pos){
  return (long)(sin(2*PI/size*pos) * offset) + offset;
}

/* Usage: sines [[SIZE] [PRECISION]] */
int main(int argc, char *argv[]){
  if(argc > 1)
    size = atoi(argv[1]);
  if(argc > 2)
    precision = atoi(argv[2]);
  offset = pow(2, precision-1)-1;
  printf("/* sine wavetable: %d %d bit values */\n", size, precision);
  puts("int table[] = {");
  int i=0;
  while(i<size){
    if(i % 16 == 0)
      printf("\n\t");
    printf("%ld", sine(i));
    if(++i < size)
      printf(", ");
  }
  puts("\n};\n");
}
