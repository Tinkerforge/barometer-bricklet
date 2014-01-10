#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'jod'; # Change to your UID

my $ipcon = IPConnection->new(); # Create IP connection
my $b = BrickletBarometer->new(&UID, $ipcon); # Create device object

# Callback function for air pressure callback (parameter has unit mbar/1000)
sub cb_air_pressure
{
    my ($air_pressure) = @_;
    print "\nAir Pressure: ".$air_pressure/1000.0." mbar\n";
}

# Callback function for altitude callback (parameter has unit cm)
sub cb_altitude
{
    my ($altitude) = @_;
    print "\nAltitude: ".$altitude/100.0." m\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set Period for air pressure and altitude callbacks to 1s (1000ms)
# Note: The air pressure and altitude callbacks are only called every second
#       if the air pressure or altitude has changed since the last call!
$b->set_air_pressure_callback_period(1000);
$b->set_altitude_callback_period(1000);

# Register air pressure callback to function cb_air_pressure
$b->register_callback($b->CALLBACK_AIR_PRESSURE, 'cb_air_pressure');

# Register altitude callback to function cb_altitude
$b->register_callback($b->CALLBACK_ALTITUDE, 'cb_altitude');

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

