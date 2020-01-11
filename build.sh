#!/usr/bin/env bash

# Install dependencies
apt-get update
# wiringpi - GPIO library for RaspberryPi
# mosquitto-clients - Network communication for mqtt
apt-get install mosquitto-clients wiringpi -y

# Create install location
mkdir -p bin
mkdir -p /opt/garage

# Build
cd ./source
gcc main.c sensors.c mqtt.c ini.c util.c -o ../bin/garage -Wall -lwiringPi -lmosquitto

# Copy files to the installation folder
cd ..
cp ./bin/garage /opt/garage
cp ./install/default.config /opt/garage
cp ./install/run.sh /opt/garage
# Manage file permissions
chmod 744 /opt/garage/run.sh

# Create daemon
sudo cp ./install/garage.service /etc/systemd/system
sudo systemctl start garage
sudo systemctl enable garage
sudo systemctl daemon-reload