#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include "../headers/util.h"

char *mqtt_topic_command_door;
char *mqtt_topic_command_temperature;
char *mqtt_topic_command_humidity;
char *mqtt_topic_status;
char *mqtt_topic_telemetry;

const char* _mqtt_project_prefix = "garage/";
const char* _mqtt_suffix_command_door = "/command/door";
const char* _mqtt_suffix_command_temperature = "/command/temperature";
const char* _mqtt_suffix_command_humidity = "/command/humidity";
const char* _mqtt_suffix_status = "/status";
const char* _mqtt_suffix_telemetry = "/telemetry";

void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{   
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq,NULL,mqtt_topic_command_door,1);
        mosquitto_subscribe(mosq,NULL,mqtt_topic_command_humidity,0);
        mosquitto_subscribe(mosq,NULL,mqtt_topic_command_temperature,0);

        printf("Connected to mosquitto with code %i\n",result);
        char* status = "{msg:connected}\0";
        mosquitto_publish(	mosq, NULL, mqtt_topic_status, strlen(status), status, 0, false);
	} else {
		fprintf(stderr, "Connect failed\n");
	}
}

void mqtt_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    if(result) // result is 0 iff we requested the disconnect
    {
        switch(mosquitto_reconnect_async(mosq))
        {
            case MOSQ_ERR_SUCCESS: printf("Reconnected to broker");
            break;
            case MOSQ_ERR_INVAL: printf("Invalid parameters while trying to reconnect to broker.");
            break;
            case MOSQ_ERR_ERRNO: printf("Error reconnecting to broker.");/*\n%s",strerror_r());*/
            break;
        }
    }

    free(mqtt_topic_command_door);
    free(mqtt_topic_command_temperature);
    free(mqtt_topic_command_humidity);
    free(mqtt_topic_status);
    free(mqtt_topic_telemetry);
}

void mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void mqtt_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Print all log messages regardless of level. */
	printf("\e[32m%s\e[0m %s\n", "LOG",str);

    // Get these on a server!
    //mosquitto_publish(	mosq, NULL, "garage/34567/log", strlen(str), str, 0, false);
}

struct mosquitto *mqtt_setup(struct mosquitto *mosq, char* id, 
    void (*mqtt_message_callback)(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message))
{
    mosquitto_lib_init();
    
    mosq = mosquitto_new(id,true,NULL);
    if(!mosq) {
            fprintf(stderr, "Error: Out of memory.\n");
            return NULL;
    }

    char* topic_prefix = concat(_mqtt_project_prefix,id);
    mqtt_topic_command_door = concat(topic_prefix,_mqtt_suffix_command_door);
    mqtt_topic_command_temperature = concat(topic_prefix,_mqtt_suffix_command_temperature);
    mqtt_topic_command_humidity = concat(topic_prefix,_mqtt_suffix_command_humidity);
    mqtt_topic_status = concat(topic_prefix,_mqtt_suffix_status);
    mqtt_topic_telemetry = concat(topic_prefix,_mqtt_suffix_telemetry);
    free(topic_prefix);

    mosquitto_log_callback_set(mosq, mqtt_log_callback);
    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_message_callback_set(mosq, mqtt_message_callback);
    mosquitto_subscribe_callback_set(mosq, mqtt_subscribe_callback);
    mosquitto_disconnect_callback_set(mosq,mqtt_disconnect_callback);
      
    return mosq;
}

int mqtt_connect(struct mosquitto *mosq, char* host, int port, int keepalive) {
    if(mosquitto_connect_async(mosq, host, port, keepalive)){
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }

    //TODO: under what circumstances is the thread/loop stopped?
    // Are multiple threads created if this line is run more than once?
    if(mosquitto_loop_start(mosq)) {
        fprintf(stderr, "Unable to start connection loop.\n");
        return 1;
    }

    return 0;
}


void mqtt_cleanup(struct mosquitto *mosq){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}



