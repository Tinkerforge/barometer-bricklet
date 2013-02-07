<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

$host = 'localhost';
$port = 4223;
$uid = 'bAc'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$b = new BrickletBarometer($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Get current air pressure (unit is mbar/1000)
$airPressure = $b->getAirPressure() / 1000.0;

echo "Air Pressure: $airPressure mbar\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
