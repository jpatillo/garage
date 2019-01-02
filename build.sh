#!/usr/bin/env bash
mkdir -p bin
cd ./source
gcc main.c sensors.c mqtt.c -o ../bin/garage -Wall -lwiringPi -lmosquitto
