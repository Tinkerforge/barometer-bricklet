<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Barometer Bricklet

// Callback function for air pressure callback
function cb_airPressure($air_pressure)
{
    echo "Air Pressure: " . $air_pressure/1000.0 . " mbar\n";
}

$ipcon = new IPConnection(); // Create IP connection
$b = new BrickletBarometer(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register air pressure callback to function cb_airPressure
$b->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE, 'cb_airPressure');

// Set period for air pressure callback to 1s (1000ms)
// Note: The air pressure callback is only called every second
//       if the air pressure has changed since the last call!
$b->setAirPressureCallbackPeriod(1000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
