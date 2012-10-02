<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

$host = 'localhost';
$port = 4223;
$uid = 'bAc'; // Change to your UID

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

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$b = new BrickletBarometer($uid); // Create device object

$ipcon->addDevice($b); // Add device to IP connection
// Don't use device before it is added to a connection

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
