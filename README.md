# Garage

Build and run with `$ ./run.sh`   
Run with `$ ./start.sh`

## Directories
* /bin - Files used to execute the module.
* /build - Files created during the build process
* /include - Header files (.h)
* /source - Source files (.cpp)


For my future sanity:
```
struct mosquitto_message{
	uint16_t mid;
	char *topic;
	uint8_t *payload;
	uint32_t payloadlen;
	int qos;
	bool retain;
};
```
