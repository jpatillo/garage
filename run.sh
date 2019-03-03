#!/usr/bin/env bash

# Make sure the pin used for the garage door relay is passive so the door will not be triggered accidentally at startup.
# It doesn't seem like this is necessary, but better safe than sorry.
gpio write 4 1

# Start the program
# TODO adjust this to match the installed location
cd /home/pi/garage/bin
./garage --config default.config
