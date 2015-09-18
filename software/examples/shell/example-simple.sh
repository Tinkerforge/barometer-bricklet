#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get current air pressure (unit is mbar/1000)
tinkerforge call barometer-bricklet $uid get-air-pressure

# Get current altitude (unit is cm)
tinkerforge call barometer-bricklet $uid get-altitude
