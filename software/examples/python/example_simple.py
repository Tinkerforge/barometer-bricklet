#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "bAc" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_barometer import Barometer

if __name__ == "__main__":
    ipcon = IPConnection(HOST, PORT) # Create IP connection to brickd

    b = Barometer(UID) # Create device object
    ipcon.add_device(b) # Add device to IP connection
    # Don't use device before it is added to a connection

    # Get current air pressure (unit is mbar/1000)
    air_pressure = b.get_air_pressure()/1000.0

    print('Air Pressure: ' + str(air_pressure) + ' mbar')

    # Get current altitude (unit is cm)
    altitude = b.get_altitude()/100.0

    print('Altitude: ' + str(altitude) + ' m')

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.destroy()
