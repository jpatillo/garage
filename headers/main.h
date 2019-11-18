#ifndef main_h_
#define main_h_

#define DHT11_PIN 3
#define RELAY_PIN 4
#define DEFAULT_ENVIRONMENT_INTERVAL 600 // 10 minutes

typedef struct
{
    int mqtt_port;
    char* mqtt_host;
    int mqtt_keepalive;
    unsigned int ports_dht11;
    unsigned int ports_relay;
    int env_update;
} configuration;

void setRelayPin(unsigned int pin);
void activateRelay(unsigned int pin);
void deactivateRelay(unsigned int pin);

#endif