#ifndef RELAY_H_
#define RELAY_H_

#include <cstdlib>

class Relay {
  unsigned int _pin;

public:
  Relay();
  Relay(unsigned int pin);
  ~Relay();

  void setPin(unsigned int pin);

  void activate();
  void deactivate();

};

#endif
