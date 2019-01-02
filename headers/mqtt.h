#include <mosquitto.h>

void mqtt_setup(struct mosquitto *mosq, char* host, int port, int keepalive);
void mqtt_cleanup(struct mosquitto *mosq);
void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void mqtt_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
