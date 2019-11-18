#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_TIMINGS	85

// Helper function that converts celsius to fahrenheit.
double getTemperatureF(double celsius);

// Returns the last temperature read by the DHT11 sensor.
double get_dht11_temperature();
// Returns the last temperature read by the DHT11 sensor in fahrenheit.
double get_dht11_temperature_f();
// Returns the last humidity read by the DHT11 sensor.
double get_dht11_humidity();
// Reads sensor data from a DHT11 sensor. To get the values, make a subsequent call to:
//  get_dht11_temperature()
//  get_dht11_humidity()
int dht11_read(unsigned int pin);
