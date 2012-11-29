#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_barometer'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'bAc' # Change to your UID

ipcon = IPConnection.new # Create IP connection
b = BrickletBarometer.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current air pressure (unit is mbar/1000)
air_pressure = b.get_air_pressure / 1000.0
puts "Air Pressure: #{air_pressure} mbar"

# Get current altitude (unit is cm)
altitude = b.get_altitude / 100.0
puts "Altitude: #{altitude} m"

puts 'Press key to exit'
$stdin.gets
