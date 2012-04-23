#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <sys/types.h>
#include <string>

class SerialCallback {
public:
  virtual ~SerialCallback(){}
  virtual int handle(unsigned char* data, size_t len) = 0;
};

class Serial {
protected:
  SerialCallback* callback;
  std::string port;
  int speed;
  bool verbose;
  volatile bool connected;
  volatile bool running;

public:
  Serial();
  Serial(const std::string& aport, int aspeed, bool averbose = true);
  virtual ~Serial();
  const std::string& getPort();
  void setPort(const std::string& aport);
  int getSpeed();
  void setSpeed(int aspeed);
  void setSerialCallback(SerialCallback* cb);
  virtual void stop();
  virtual void start();
  virtual bool isConnected();
  virtual void run();
  virtual int connect() = 0;
  virtual int disconnect() = 0;
  virtual size_t readSerial(unsigned char* data, size_t len) = 0;
  virtual size_t writeSerial(unsigned char* data, size_t len) = 0;

  static Serial* createSerial();
protected:
  virtual void log(unsigned char* data, size_t len);
};

#endif  // __SERIAL_H__
