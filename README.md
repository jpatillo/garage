# Garage

The garage project is a control and monitoring service for garages.

This project has been migrated from C++ to C.

TODO: Move to gRPC so we can post to firebase and get some good security.

Current capabilities include:

- open/close door (single door)
- post temperature and humidity on an interval
- receive requests to post temperature and humidity

## HARDWARE

- Pi Zero-W (Raspbian Stretch)
- DHT11 Temperature and Humidity sensor
- 2-Channel relay

TODO: gracefully quit if running as a service
TODO: tls

- Build with `$ ./build.sh`
- Run with `$ ./run.sh`
- As a daemon `$ ./daemon.sh`

## Directories

- /bin - Files used to execute the module.
- /build - Files created during the build process
- /include - Header files (.h)
- /source - Source files (.c)

## External Code

Config file code from <https://github.com/benhoyt/inih.> Consists of ini.h and ini.c

## MQTT

The mosquitto broker and client libraries are being used in the project. The IoT devices are currently running single threaded.

Subscribes to:

- garage/temperature
- garage/humidity
- garage/door

Publishes to:

- 

For my future sanity:

```c
struct mosquitto_message {
    uint16_t mid;
    char *topic;
    uint8_t *payload;
    uint32_t payloadlen;
    int qos;
    bool retain;
};
```
