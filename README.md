# Garage

The garage project is a control and monitoring service for garages. This module is the garage controller itself and performs the open/close function.

Server Backend <https://github.com/jpatillo/garage-api>   
Android Project <https://github.com/jpatillo/garage-android>

This project has been migrated from C++ to C.

Current capabilities include:

- receive requests to open/close door (single door)
- post temperature and humidity on an interval
- receive requests to post temperature and humidity

## TODO

- tls
- change hard-coded device id
  - DS18B20 - connect to the board to provide a unique id
- ability for user to change telemetry reporting interval

- Once the hardware is somewhere near being finalized, a qr-code with the device id can be put on the housing. This will allow the app to scan it and add it.
- Set the device hosts/hostname to the device id for easy network identification
- Need a way to do user setup on the device to connect it to wifi.
  - Start the device as an AP with a webpage that the user can log into and change settings via wifi?

## HARDWARE

- Pi Zero-W (Raspbian Stretch)
- DHT11 Temperature and Humidity sensor
- 2-Channel relay

## BUILD

- Build and install with `$ ./build.sh`
- Test with `$ ./testrun.sh`

## Directories

- /install - Files used to install and run the application.
- /build - Files created during the build process
- /headers - Header files (.h)
- /source - Source files (.c)

## Library Code

Config file code from <https://github.com/benhoyt/inih.> Consists of ini.h and ini.c

## MQTT

The mosquitto broker and client libraries are being used in the project. Clients spawn a new thread (handled by the library) to handle requests.

Subscribes to:

- garage/_clientId_/command/temperature
- garage/_clientId_/command/humidity
- garage/_clientId_/command/door

Publishes to:

- garage/_clientId_/status
- garage/_clientId_/telemetry
