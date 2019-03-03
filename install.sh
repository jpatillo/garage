#!/usr/bin/env bash

apt-get update

# git - git repo
# wiringpi - GPIO library for RaspberryPi
apt-get install mosquitto-clients git wiringpi -y

# mosquitto-clients - MQTT Client for sending messages.
apt-get install mosquitto-clients -y
