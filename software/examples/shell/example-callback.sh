#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# set period for barometer callback to 1s (1000ms)
# note: the air-pressure callback is only called every second if the
#       air pressure has changed since the last call!
tinkerforge call barometer-bricklet $uid set-air-pressure-callback-period 1000

# handle incoming air-pressure callbacks (unit is mbar/1000)
tinkerforge dispatch barometer-bricklet $uid air-pressure
