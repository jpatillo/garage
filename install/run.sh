#!/usr/bin/env bash
cd /opt/garage

# Make sure the pin used for the garage door relay is passive so the door will not be triggered accidentally at startup.
# It doesn't seem like this is necessary, but better safe than sorry.
gpio write 4 1

# Start the program
./garage --config default.config
