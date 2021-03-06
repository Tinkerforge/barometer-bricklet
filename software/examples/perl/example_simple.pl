#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Barometer Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $b = Tinkerforge::BrickletBarometer->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current air pressure
my $air_pressure = $b->get_air_pressure();
print "Air Pressure: " . $air_pressure/1000.0 . " hPa\n";

# Get current altitude
my $altitude = $b->get_altitude();
print "Altitude: " . $altitude/100.0 . " m\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
