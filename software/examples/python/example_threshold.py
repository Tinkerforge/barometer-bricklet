#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "bAc" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_barometer import Barometer

# Callback for air pressure greater than 1025 mbar
def cb_reached(air_pressure):
    print('We have ' + str(air_pressure/1000.0) + ' mbar.')
    print('Enjoy the potentially good weather!')

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    b = Barometer(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    b.set_debounce_period(10000)

    # Register threshold reached callback to function cb_reached
    b.register_callback(b.CALLBACK_AIR_PRESSURE_REACHED, cb_reached)

    # Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
    b.set_air_pressure_callback_threshold('>', 1025*1000, 0)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
