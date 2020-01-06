#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

#include "../headers/main.h"
#include "../headers/mqtt.h"
#include "../headers/sensors.h"
#include "../headers/ini.h"

int8_t volatile seagulls = 1; // loop control
time_t environment_timer;

// MQTT
struct mosquitto *mosq = NULL;
configuration config;
char* config_file = "default.config";


static int configHandler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("mqtt", "port")) {
        pconfig->mqtt_port = atoi(value);
    } else if (MATCH("mqtt", "host")) {
        pconfig->mqtt_host = strdup(value);
    } else if (MATCH("mqtt", "keepalive")) {
        pconfig->mqtt_keepalive = atoi(value);
    } else if (MATCH("ports", "dht11")) {
        pconfig->ports_dht11 = atoi(value);
    } else if (MATCH("ports", "relay")) {
        pconfig->ports_relay = atoi(value);
    } else if (MATCH("environment", "update")) {
        pconfig->env_update = atoi(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

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

    if ( wiringPiSetup() == -1 ) {
            fprintf(stderr, "\e[31mWiringPi is not installed. Bye.\e[0m\n");
            exit( 1 );
    }

    // Set the config to defaults. A config file or command line argument is required to get a host for the mqtt broker.
    config = (configuration) {1883,NULL,60,DHT11_PIN,RELAY_PIN,DEFAULT_ENVIRONMENT_INTERVAL};
    // Try to update the config from a file
    if (ini_parse(config_file, configHandler, &config) < 0) {
        printf("\e[31mCan't load \'%s\'.\e[0m Using defaults.\n", config_file);
    }

    // We require a host to run.
    if(config.mqtt_host==NULL) {
      fprintf(stderr, "\e[31mNo host provided. Bye.\e[0m\n");
      exit(1);
    }

    setRelayPin(RELAY_PIN);
    
    
   mosq = mqtt_setup(mosq, config.mqtt_host, config.mqtt_port, config.mqtt_keepalive);
  

  // Start timers
  time(&environment_timer);
}



void loop() {

  if(difftime(time(0),environment_timer) > config.env_update) {
    dht11_read(DHT11_PIN);
    delay(2000);
   
    char display[80];
    char* topic = "garage/34567/telemetry";
    int size = sprintf(display,"{Humidity: %f, Temperature: %ff}",get_dht11_humidity(),get_dht11_temperature_f());
    int pr = mosquitto_publish(	mosq, NULL, topic, size, display, 0, false);
    
    if(pr!=MOSQ_ERR_SUCCESS){
      char* msg;
      switch(pr){
        case MOSQ_ERR_INVAL:
        msg = "Invalid Value";
        break;
        case MOSQ_ERR_NOMEM:
        msg = "No Memory";
        break;
        case MOSQ_ERR_NO_CONN:
        msg = "No Connection";
        break;
        case MOSQ_ERR_PROTOCOL:
        msg = "Protocol Error";
        break;
        case MOSQ_ERR_PAYLOAD_SIZE:
        msg = "Payload Size";
        break;
        default:
        msg = "Unknown Error";
      }
      printf("Error publishing to broker: %s\n",msg);
    }

    time(&environment_timer);
  }
}

void print_args_error(char* name) {
  fprintf(stderr, "usage: %s [--config]\n", name);
  exit(1);
}
void parse_args(int argc, char* argv[]){
  if(argc<2)return;
  for(int c=1;c<argc-1;c++){
    if(strcmp(argv[c],"--config")==0) {
      if(c+1<=argc-1) config_file = argv[++c];
      else print_args_error(argv[0]);
    }
    if(strcmp(argv[c],"-h")==0) {
      if(c+1<=argc-1) config.mqtt_host = argv[++c];
      else print_args_error(argv[0]);
    }
  }
}



int main( int argc, char* argv[] )
{
    parse_args(argc, argv);
    setup();
    while(seagulls) { loop(); }
    cleanup();
    return(0);
}