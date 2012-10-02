<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

$host = 'localhost';
$port = 4223;
$uid = 'bAc'; // Change to your UID

// Callback for air pressure greater than 1025 mbar
function cb_reached($airPressure)
{
    echo "We have " . $airPressure / 1000.0 . " mbar.\n";
    echo "Enjoy the potentially good weather!\n";
}

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$b = new BrickletBarometer($uid); // Create device object

$ipcon->addDevice($b); // Add device to IP connection
// Don't use device before it is added to a connection

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$b->setDebouncePeriod(10000);

// Register threshold reached callback to function cb_reached
$b->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE_REACHED, 'cb_reached');

// Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
$b->setAirPressureCallbackThreshold('>', 1025*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
