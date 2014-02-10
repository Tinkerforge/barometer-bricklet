#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'jod'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $b = Tinkerforge::BrickletBarometer->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current air pressure (unit is mbar/1000)
my $air_pressure = $b->get_air_pressure()/1000.0;

print "\nAir Pressure: $air_pressure mbar\n";

# Get current altitude (unit is cm)
my $altitude = $b->get_altitude()/100.0;

print "\nAltitude: $altitude m\n";

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

