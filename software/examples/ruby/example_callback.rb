#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_barometer'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Barometer Bricklet

ipcon = IPConnection.new # Create IP connection
b = BrickletBarometer.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register air pressure callback
b.register_callback(BrickletBarometer::CALLBACK_AIR_PRESSURE) do |air_pressure|
  puts "Air Pressure: #{air_pressure/1000.0} mbar"
end

# Set period for air pressure callback to 1s (1000ms)
# Note: The air pressure callback is only called every second
#       if the air pressure has changed since the last call!
b.set_air_pressure_callback_period 1000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
