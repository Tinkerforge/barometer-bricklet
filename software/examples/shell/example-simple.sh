#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=d99

# get current air pressure (unit is mbar/1000)
tinkerforge call barometer-bricklet $uid get-air-pressure
