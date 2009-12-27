#ifndef _MCP492XCONTROLLER_H_
#define _MCP492XCONTROLLER_H_

#include <stdint.h>
/* #include "device.h" */

class MCP492xController {

 public:
  void init();

  void send(uint8_t data);

  void send(uint8_t high, uint8_t low);

  void send(uint16_t data);
};

#endif /* _MCP492XCONTROLLER_H_ */
