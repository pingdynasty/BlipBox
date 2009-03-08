#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

class Protocol {
public:
  Protocol(){}
  virtual void init() = 0;
  virtual void process() = 0;
};


#endif /* _PROTOCOL_H_ */
