#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Barometer Bricklet

# Handle incoming air pressure callbacks
tinkerforge dispatch barometer-bricklet $uid air-pressure &

# Set period for air pressure callback to 1s (1000ms)
# Note: The air pressure callback is only called every second
#       if the air pressure has changed since the last call!
tinkerforge call barometer-bricklet $uid set-air-pressure-callback-period 1000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
