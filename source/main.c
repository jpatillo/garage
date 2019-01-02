#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

#include "../headers/main.h"
#include "../headers/mqtt.h"
#include "../headers/sensors.h"

int8_t volatile seagulls = 1; // loop control
time_t environment_timer;
unsigned int environment_interval = DEFAULT_ENVIRONMENT_INTERVAL;

// MQTT
struct mosquitto *mosq = NULL;
char *mqtt_host = "bullwinkle.jamespatillo.com";
int mqtt_port = 1883;
int mqtt_keepalive = 60;



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
    
    
   mqtt_setup(mosq, mqtt_host, mqtt_port, mqtt_keepalive);
  

  // Start timers
  time(&environment_timer);
}



void loop() {

  if(difftime(time(0),environment_timer) > environment_interval) {
    dht11_read(DHT11_PIN);
    delay(2000);
   
    char display[80];
    sprintf(display,"{Humidity: %f, Temperature: %f}",get_dht11_temperature(),get_dht11_humidity());
    
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