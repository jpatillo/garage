#ifndef CONFIG_H_
#define CONFIG_H_

#define DEFAULT_CONFIG_FILE "default.config"

class config {
    int mqtt_port;
    char* mqtt_host;
    int mqtt_keepalive;
    unsigned int dht11_pin;
    unsigned int relay_pin;
    unsigned int environment_interval;

    public:
    config();




    bool has_mqtt_host();

};

#endif