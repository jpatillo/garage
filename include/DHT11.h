// Class for reading and making requests from a DHT11.
#ifndef DHT11_H_
#define DHT11_H_

class DHT11 {
  unsigned int _pin;
  double humidity, temperature;
public:
  DHT11();
  DHT11(unsigned int pin);

  void read_dht11_dat();

  double getHumidity();
  double getTemperature();
  double getTemperatureF();

  void setPin(unsigned int pin);
};

#endif
