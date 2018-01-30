#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Barometer Bricklet

# Callback subroutine for air pressure callback
sub cb_air_pressure
{
    my ($air_pressure) = @_;

    print "Air Pressure: " . $air_pressure/1000.0 . " mbar\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $b = Tinkerforge::BrickletBarometer->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register air pressure callback to subroutine cb_air_pressure
$b->register_callback($b->CALLBACK_AIR_PRESSURE, 'cb_air_pressure');

# Set period for air pressure callback to 1s (1000ms)
# Note: The air pressure callback is only called every second
#       if the air pressure has changed since the last call!
$b->set_air_pressure_callback_period(1000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
