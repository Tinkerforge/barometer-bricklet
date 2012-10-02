#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "bAc" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_barometer import Barometer

# Callback function for air pressure callback (parameter has unit mbar/1000)
def cb_air_pressure(air_pressure):
    print('Air Pressure: ' + str(air_pressure/1000.0) + ' mbar')

# Callback function for altitude callback (parameter has unit cm)
def cb_altitude(altitude):
    print('Altitude: ' + str(altitude/100.0) + ' m')

if __name__ == "__main__":
    ipcon = IPConnection(HOST, PORT) # Create IP connection to brickd

    b = Barometer(UID) # Create device object
    ipcon.add_device(b) # Add device to IP connection
    # Don't use device before it is added to a connection

    # Set Period for air pressure and altitude callbacks to 1s (1000ms)
    # Note: The air pressure and altitude callbacks are only called every second
    #       if the air pressure or altitude has changed since the last call!
    b.set_air_pressure_callback_period(1000)
    b.set_altitude_callback_period(1000)

    # Register air pressure callback to function cb_air_pressure
    b.register_callback(b.CALLBACK_AIR_PRESSURE, cb_air_pressure)

    # Register altitude callback to function cb_altitude
    b.register_callback(b.CALLBACK_ALTITUDE, cb_altitude)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.destroy()
