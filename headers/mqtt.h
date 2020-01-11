#ifndef MQTT_H_
#define MQTT_H_

#include <mosquitto.h>

extern char *mqtt_topic_command_door;
extern char *mqtt_topic_command_temperature;
extern char *mqtt_topic_command_humidity;
extern char *mqtt_topic_status;
extern char *mqtt_topic_telemetry;

struct mosquitto *mqtt_setup(struct mosquitto *mosq, char* id,
    void (*mqtt_message_callback)(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message));
int mqtt_connect(struct mosquitto *mosq, char* host, int port, int keepalive);
void mqtt_cleanup(struct mosquitto *mosq);

#endif