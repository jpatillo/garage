#!/usr/bin/env bash

#clean the build folder
rm -R bin

# Install dependencies
apt-get update
# wiringpi - GPIO library for RaspberryPi
# mosquitto-clients - Network communication for mqtt
apt-get install mosquitto-clients wiringpi libmosquitto-dev libmosquittopp-dev libssl-dev -y

# Create install location
mkdir -p bin
mkdir -p /opt/garage

# Compile
./compile.sh

# Copy files to the installation folder

cp ./bin/garage /opt/garage/garage
cp ./install/default.config /opt/garage/default.config
cp ./install/run.sh /opt/garage/run.sh
# Manage file permissions
chmod 755 /opt/garage/run.sh
chmod 755 /opt/garage/garage

# Create daemon
sudo cp ./install/garage.service /etc/systemd/system/garage.service
sudo chmod 755 /etc/systemd/system/garage.service
# Stop the daemon if it is already running.
sudo systemctl stop garage
# Start daemon and set to run on boot
sudo systemctl start garage
sudo systemctl enable garage
sudo systemctl daemon-reload