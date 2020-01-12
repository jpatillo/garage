#!/usr/bin/env bash

# Create install location
mkdir -p test

# Compile
./compile.sh

# Copy files to the installation folder
cd ..
cp ./install/default.config test

# Create daemon
cd test

# Make sure the pin used for the garage door relay is passive so the door will not be triggered accidentally at startup.
# It doesn't seem like this is necessary, but better safe than sorry.
gpio write 4 1

# Start the program
./garage --config default.config
