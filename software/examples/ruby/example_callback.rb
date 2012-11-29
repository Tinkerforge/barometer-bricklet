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

# Set Period for air pressure and altitude callbacks to 1s (1000ms)
# Note: The air pressure and altitude callbacks are only called every second
#       if the air pressure or altitude has changed since the last call!
b.set_air_pressure_callback_period 1000
b.set_altitude_callback_period 1000

# Register air pressure callback (parameter has unit mbar/1000)
b.register_callback(BrickletBarometer::CALLBACK_AIR_PRESSURE) do |air_pressure|
  puts "Air Pressure: #{air_pressure/1000.0} mbar"
end

# Register altitude callback (parameter has unit cm)
b.register_callback(BrickletBarometer::CALLBACK_ALTITUDE) do |altitude|
  puts "Altitude: #{altitude/100.0} m"
end

puts 'Press key to exit'
$stdin.gets
