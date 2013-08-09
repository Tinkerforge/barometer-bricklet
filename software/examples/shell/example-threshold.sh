#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call barometer-bricklet $uid set-debounce-period 10000

# configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
tinkerforge call barometer-bricklet $uid set-air-pressure-callback-threshold greater 1025000 0

# handle incoming air-pressure-reached callbacks (unit is mbar/1000)
tinkerforge dispatch barometer-bricklet $uid air-pressure-reached\
 --execute "echo We have {air-pressure} mbar/1000. Enjoy the potentially good weather!"
