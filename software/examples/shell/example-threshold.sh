#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call barometer-bricklet $uid set-debounce-period 10000

# Handle incoming air pressure reached callbacks (parameter has unit mbar/1000)
tinkerforge dispatch barometer-bricklet $uid air-pressure-reached\
 --execute "echo Air Pressure: {air_pressure} mbar/1000. Enjoy the potentially good weather!" &

# Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
tinkerforge call barometer-bricklet $uid set-air-pressure-callback-threshold greater 1025000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
