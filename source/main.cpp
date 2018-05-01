#include <wiringPi.h>
#include <iostream>
#include <sstream>
#include <signal.h>         // Handle Ctrl-C
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bugnet.h"
#include "DHT11.h"
#include "Relay.h"

using namespace std;

const unsigned int DEFAULT_ENVIRONMENT_INTERVAL = 600; // 10 minutes


int8_t volatile seagulls = 1; // loop control
DHT11* sensor;
Relay* doorctrl;
bugnet* comms;
time_t environment_timer;
unsigned int environment_interval = DEFAULT_ENVIRONMENT_INTERVAL;

void cleanup() {
  delete sensor;
  delete doorctrl;
  delete comms;
}

// Called when user presses Ctrl-C
void interruptHandler(int sig) {
  if(sig==SIGINT) {
    // End the program loop.
    seagulls = 0;
  }
}

void bugnetHandler(string topic, string message) {
  cout << "Message received on " << topic << "." << endl;

  // Get temperature
  if(topic=="garage/temperature") {
    std::ostringstream strs;
    strs << "Temperature " << sensor->getTemperatureF();

    comms->do_publish("test", strs.str());
  }
  // Get humidity
  if(topic=="garage/humidity") {
    std::ostringstream strs;
    strs << "Humidity " << sensor->getHumidity();

    comms->do_publish("test", strs.str());
  }
  // Signal Relay
  if(topic=="garage/door") {
    doorctrl->activate();
    delay( 1000 );
    // Make sure to deactivate so the doorbell button will work.
    doorctrl->deactivate();
  }

}


void setup() {
  // Handle ctrl+c
  signal (SIGINT, interruptHandler);

  if ( wiringPiSetup() == -1 )
		exit( 1 );

  sensor = new DHT11(3);
  doorctrl = new Relay(4);
  comms = new bugnet("Garage Control", "bullwinkle.jamespatillo.com", 1883);
  comms->set_message_callback(bugnetHandler);
  comms->subscribe(NULL,"garage/door");
  comms->subscribe(NULL,"garage/humidity");
  comms->subscribe(NULL,"garage/temperature");

  // Start timers
  time(&environment_timer);
}

void loop() {
  if(difftime(time(0),environment_timer) > environment_interval) {
    sensor->read_dht11_dat();
    delay(2000);
    std::ostringstream strs;
    strs << "{Humidity:" << sensor->getHumidity() << ",Temperature:" << sensor->getTemperature() << "}";
    comms->do_publish("test", strs.str());
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
