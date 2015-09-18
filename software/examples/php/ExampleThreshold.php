<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for air pressure reached callback (parameter has unit mbar/1000)
function cb_airPressureReached($air_pressure)
{
    echo "Air Pressure: " . $air_pressure/1000.0 . " mbar\n";
    echo "Enjoy the potentially good weather!\n";
}

$ipcon = new IPConnection(); // Create IP connection
$b = new BrickletBarometer(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$b->setDebouncePeriod(10000);

// Register air pressure reached callback to function cb_airPressureReached
$b->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE_REACHED, 'cb_airPressureReached');

// Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
$b->setAirPressureCallbackThreshold('>', 1025*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
