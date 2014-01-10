#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletBarometer;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'jod'; # Change to your UID

my $ipcon = IPConnection->new(); # Create IP connection
my $b = BrickletBarometer->new(&UID, $ipcon); # Create device object

# Callback for air pressure greater than 1025 mbar
sub cb_reached
{
    my ($air_pressure) = @_;
    print "\nWe have ".$air_pressure/1000.0." mbar.";
    print "\nEnjoy the potentially good weather!\n"
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$b->set_debounce_period(10000);

# Register threshold reached callback to function cb_reached
$b->register_callback($b->CALLBACK_AIR_PRESSURE_REACHED, 'cb_reached');

# Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
$b->set_air_pressure_callback_threshold('>', 1025*1000, 0);

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

