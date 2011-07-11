#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C"{
#endif

void beginSerial(long);
void serialWrite(unsigned char);

unsigned long millis(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* _SERIAL_H_ */
