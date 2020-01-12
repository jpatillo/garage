#!/usr/bin/env bash

cd ./source
gcc main.c sensors.c mqtt.c ini.c util.c -o ../bin/garage -Wall -lwiringPi -lmosquitto
cd ..