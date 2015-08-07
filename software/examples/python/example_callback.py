#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_barometer import BrickletBarometer

# Callback function for air pressure callback (parameter has unit mbar/1000)
def cb_air_pressure(air_pressure):
    print('Air Pressure: ' + str(air_pressure/1000.0) + ' mbar')

# Callback function for altitude callback (parameter has unit cm)
def cb_altitude(altitude):
    print('Altitude: ' + str(altitude/100.0) + ' m')

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    b = BrickletBarometer(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set period for air pressure callback to 1s (1000ms)
    # Note: The air pressure callback is only called every second
    #       if the air pressure has changed since the last call!
    b.set_air_pressure_callback_period(1000)

    # Register air pressure callback to function cb_air_pressure
    b.register_callback(b.CALLBACK_AIR_PRESSURE, cb_air_pressure)

    # Set period for altitude callback to 1s (1000ms)
    # Note: The altitude callback is only called every second
    #       if the altitude has changed since the last call!
    b.set_altitude_callback_period(1000)

    # Register altitude callback to function cb_altitude
    b.register_callback(b.CALLBACK_ALTITUDE, cb_altitude)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
