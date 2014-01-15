<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'bAc'; // Change to your UID

// Callback function for air pressure callback (parameter has unit mbar/1000)
function cb_airPressure($airPressure)
{
    echo "Air Pressure: " . $airPressure / 1000.0 . " mbar\n";
}

// Callback function for altitude callback (parameter has unit cm)
function cb_altitude($altitude)
{
    echo "Altitude: " . $altitude / 100.0 . " m\n";
}

$ipcon = new IPConnection(); // Create IP connection
$b = new BrickletBarometer(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set Period for air pressure and altitude callbacks to 1s (1000ms)
// Note: The air pressure and altitude callbacks are only called every second
//       if the air pressure or altitude has changed since the last call!
$b->setAirPressureCallbackPeriod(1000);
$b->setAltitudeCallbackPeriod(1000);

// Register air pressure callback to function cb_airPressure
$b->registerCallback(BrickletBarometer::CALLBACK_AIR_PRESSURE, 'cb_airPressure');

// Register air pressure callback to function cb_altitude
$b->registerCallback(BrickletBarometer::CALLBACK_ALTITUDE, 'cb_altitude');

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
