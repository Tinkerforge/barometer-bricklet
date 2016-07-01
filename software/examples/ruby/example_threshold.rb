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

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
b.set_debounce_period 10000

# Register air pressure reached callback (parameter has unit mbar/1000)
b.register_callback(BrickletBarometer::CALLBACK_AIR_PRESSURE_REACHED) do |air_pressure|
  puts "Air Pressure: #{air_pressure/1000.0} mbar"
  puts 'Enjoy the potentially good weather!'
end

# Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
b.set_air_pressure_callback_threshold '>', 1025*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
