#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_TIMINGS	85

double getTemperatureF(double celsius);
int dht11_read(unsigned int pin, double* temperature, double* humidity);
