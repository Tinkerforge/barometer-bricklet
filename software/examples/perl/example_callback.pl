#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $b = Tinkerforge::BrickletBarometer->new(&UID, $ipcon); # Create device object

# Callback subroutine for air pressure callback (parameter has unit mbar/1000)
sub cb_air_pressure
{
    my ($air_pressure) = @_;

    print "Air Pressure: " . $air_pressure/1000.0 . " mbar\n";
}

# Callback subroutine for altitude callback (parameter has unit cm)
sub cb_altitude
{
    my ($altitude) = @_;

    print "Altitude: " . $altitude/100.0 . " m\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set period for air pressure callback to 1s (1000ms)
# Note: The air pressure callback is only called every second
#       if the air pressure has changed since the last call!
$b->set_air_pressure_callback_period(1000);

# Register air pressure callback to subroutine cb_air_pressure
$b->register_callback($b->CALLBACK_AIR_PRESSURE, 'cb_air_pressure');

# Set period for altitude callback to 1s (1000ms)
# Note: The altitude callback is only called every second
#       if the altitude has changed since the last call!
$b->set_altitude_callback_period(1000);

# Register altitude callback to subroutine cb_altitude
$b->register_callback($b->CALLBACK_ALTITUDE, 'cb_altitude');

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
