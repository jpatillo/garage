#ifndef main_h_
#define main_h_

#define DHT11_PIN 3
#define RELAY_PIN 4
#define DEFAULT_ENVIRONMENT_INTERVAL 10//600; // 10 minutes


void setRelayPin(unsigned int pin);
void activateRelay(unsigned int pin);
void deactivateRelay(unsigned int pin);

#endif