<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletBarometer.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletBarometer;

$host = 'localhost';
$port = 4223;
$uid = 'bAc'; // Change to your UID

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$b = new BrickletBarometer($uid); // Create device object

$ipcon->addDevice($b); // Add device to IP connection
// Don't use device before it is added to a connection

// Get current air pressure (unit is mbar/1000)
$airPressure = $b->getAirPressure() / 1000.0;

echo "Air Pressure: $airPressure mbar\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->destroy();

?>
