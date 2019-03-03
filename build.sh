#!/usr/bin/env bash
mkdir -p bin
cd ./source
gcc main.c sensors.c mqtt.c ini.c -o ../bin/garage -Wall -lwiringPi -lmosquitto

cd ..
cp ./default.config ./bin