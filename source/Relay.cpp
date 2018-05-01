#include <wiringPi.h>

#include "Relay.h"

Relay::Relay(){}
Relay::Relay(unsigned int pin){setPin(pin);}
Relay::~Relay(){}

void Relay::setPin(unsigned int pin){
  _pin=pin;

  pinMode( _pin, OUTPUT );
}

void Relay::activate() {
  digitalWrite( _pin, LOW );
}

void Relay::deactivate() {
  digitalWrite( _pin, HIGH );
}
