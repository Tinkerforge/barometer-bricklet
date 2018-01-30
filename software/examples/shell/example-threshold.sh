#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Barometer Bricklet

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call barometer-bricklet $uid set-debounce-period 10000

# Handle incoming air pressure reached callbacks
tinkerforge dispatch barometer-bricklet $uid air-pressure-reached\
 --execute "echo Air Pressure: {air_pressure}/1000 mbar. Enjoy the potentially good weather!" &

# Configure threshold for air pressure "greater than 1025 mbar"
tinkerforge call barometer-bricklet $uid set-air-pressure-callback-threshold threshold-option-greater 1025000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
