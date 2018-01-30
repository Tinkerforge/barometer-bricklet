var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Barometer Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var b = new Tinkerforge.BrickletBarometer(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        b.setDebouncePeriod(10000);

        // Configure threshold for air pressure "greater than 1025 mbar"
        b.setAirPressureCallbackThreshold('>', 1025*1000, 0);
    }
);

// Register air pressure reached callback
b.on(Tinkerforge.BrickletBarometer.CALLBACK_AIR_PRESSURE_REACHED,
    // Callback function for air pressure reached callback
    function (airPressure) {
        console.log('Air Pressure: ' + airPressure/1000.0 + ' mbar');
        console.log('Enjoy the potentially good weather!');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
