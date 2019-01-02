#include <stdio.h>
#include <wiringPi.h>

#include "../headers/mqtt.h"
#include "../headers/sensors.h"

const unsigned int DEFAULT_ENVIRONMENT_INTERVAL = 600; // 10 minutes

#define DHT11_PIN 3
#define RELAY_PIN 4

int8_t volatile seagulls = 1; // loop control
time_t environment_timer;
unsigned int environment_interval = DEFAULT_ENVIRONMENT_INTERVAL;
struct mosquitto *mosq = NULL;
char *host = "bullwinkle.jamespatillo.com";
int port = 1883;
int keepalive = 60;
double *dht11_temp, *dht11_humidity;


void cleanup() {    

    mqtt_cleanup(mosq);

}

// Called when user presses Ctrl-C
void interruptHandler(int sig) {
  if(sig==SIGINT) {
    // End the program loop.
    seagulls = 0;
  }
}

void setRelayPin(unsigned int pin){
  pinMode( pin, OUTPUT );
}

void activateRelay(unsigned int pin) {
  digitalWrite( pin, LOW );
}

void deactivateRelay(unsigned int pin) {
  digitalWrite( pin, HIGH );
}


void setup() {
    // Handle ctrl+c
    signal (SIGINT, interruptHandler);

    if ( wiringPiSetup() == -1 )
            exit( 1 );

    setRelayPin(RELAY_PIN);
    
    
   mqtt_setup(mosq, host, port, keepalive);
  

  // Start timers
  time(&environment_timer);
}



void loop() {

  if(difftime(time(0),environment_timer) > environment_interval) {
    dht11_read(DHT11_pin,dht11_temp,dht11_humidity);
    delay(2000);
   
    char display[80];
    sprintf(display,"{Humidity: %d, Temperature: %d}",*dht11_temp,*dht11_humidity);
    
    mosquitto_publish(	mosq, NULL, "test", strlen(display), display, 0, false);

    time(&environment_timer);
  }
}

int main( void )
{
    setup();
    while(seagulls) { loop(); }
    cleanup();
    return(0);
}