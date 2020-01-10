#include "../headers/mqtt.h"
#include "../headers/main.h"
#include "../headers/sensors.h"

//TODO: it may not be necessary to have mosq as a param. The reference had to be returned because the param was not retaining the reference.
struct mosquitto *mqtt_setup(struct mosquitto *mosq, char* host, int port, int keepalive){
    mosquitto_lib_init();
    
    mosq = mosquitto_new("34567",true,NULL);
    if(!mosq) {
            fprintf(stderr, "Error: Out of memory.\n");
            return NULL;
        }
    mosquitto_log_callback_set(mosq, mqtt_log_callback);
    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_message_callback_set(mosq, mqtt_message_callback);
    mosquitto_subscribe_callback_set(mosq, mqtt_subscribe_callback);
    mosquitto_disconnect_callback_set(mosq,mqtt_disconnect_callback);

    if(mosquitto_connect_async(mosq, host, port, keepalive)){
        fprintf(stderr, "Unable to connect.\n");
        return NULL;
    }

    mosquitto_loop_start(mosq);
      
    return mosq;
}

void mqtt_cleanup(struct mosquitto *mosq){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", message->topic, (char*)message->payload);

        char payload[80];

        if(strcmp(message->topic,"garage/34567/command/temperature")==0){
            sprintf(payload,"Temperature %f", getTemperatureF(get_dht11_temperature()));
            mosquitto_publish(	mosq, NULL, "garage/34567/telemetry/temperature", strlen(payload), payload, 0, false);
        }
        if(strcmp(message->topic,"garage/34567/command/humidity")==0){
            sprintf(payload,"Humidity %f", get_dht11_humidity());
            mosquitto_publish(	mosq, NULL, "garage/34567/telemetry/humidity", strlen(payload), payload, 0, false);
        }
        if(strcmp(message->topic,"garage/34567/command/door")==0){
            activateRelay(RELAY_PIN);
            delay( 1000 );
            // Make sure to deactivate so the doorbell button will work.
            deactivateRelay(RELAY_PIN);
            //TODO: add door sensors so we know what the door is doing. Then we can error check and publish the status

            char* status = "{msg:'Door Activated'}\0";
            mosquitto_publish(	mosq, NULL, "garage/34567/status", strlen(status), status, 0, false);
        }

	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);

}

void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq,NULL,"garage/34567/command/door",1);
        mosquitto_subscribe(mosq,NULL,"garage/34567/command/humidity",0);
        mosquitto_subscribe(mosq,NULL,"garage/34567/command/temperature",0);

        printf("Connected to mosquitto with code %i\n",result);
        char* status = "{msg:connected}\0";
        mosquitto_publish(	mosq, NULL, "garage/34567/status", strlen(status), status, 0, false);
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
    mosquitto_publish(	mosq, NULL, "garage/34567/log", strlen(str), str, 0, false);
}