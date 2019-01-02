#include "../headers/mqtt.h"

void mqtt_setup(struct mosquitto *mosq, char* host, int port, int keepalive){
     mosquitto_lib_init();
    mosq *mosquitto_new(NULL,true,NULL);
    if(!mosq){
            fprintf(stderr, "Error: Out of memory.\n");
            return 1;
        }
    mosquitto_log_callback_set(mosq, mqtt_log_callback);
    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_message_callback_set(mosq, mqtt_message_callback);
    mosquitto_subscribe_callback_set(mosq, mqtt_subscribe_callback);

    if(mosquitto_connect(mosq, host, port, keepalive)){
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }

    mosquitto_loop_forever(mosq, -1, 1);
  
  mosquitto_subscribe(mosq,NULL,"garage/door",0);
  mosquitto_subscribe(mosq,NULL,"garage/humidity",0);
  mosquitto_subscribe(mosq,NULL,"garage/temperature",0);
}

void mqtt_cleanup(struct mosquitto *mosq){
    mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", message->topic, message->payload);

        char payload[80];

        if(strcmp(message->topic,"garage/temperature")==0){
            sprintf(payload,"Temperature %d", getTemperatureF(*dht11_temp));
            mosquitto_publish(	mosq, NULL, "test", strlen(payload), payload, 0, false);
        }
        if(strcmp(message->topic,"garage/humidity")==0){
            sprintf(payload,"Humidityure %d", *dht11_humidity);
            mosquitto_publish(	mosq, NULL, "test", strlen(payload), payload, 0, false);
        }
        if(strcmp(message->topic,"garage/door")==0){
            doorctrl->activate();
            delay( 1000 );
            // Make sure to deactivate so the doorbell button will work.
            doorctrl->deactivate();
        }

	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);

}

void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		//mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);

        printf("Connected to mosquitto with code %i",result);
	}else{
		fprintf(stderr, "Connect failed\n");
	}
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
	printf("%s\n", str);
}