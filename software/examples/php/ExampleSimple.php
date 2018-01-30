<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Barometer Bricklet

$ipcon = new IPConnection(); // Create IP connection
$b = new BrickletBarometer(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current air pressure
$air_pressure = $b->getAirPressure();
echo "Air Pressure: " . $air_pressure/1000.0 . " mbar\n";

// Get current altitude
$altitude = $b->getAltitude();
echo "Altitude: " . $altitude/100.0 . " m\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
