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
        // Set period for air pressure callback to 1s (1000ms)
        // Note: The air pressure callback is only called every second
        //       if the air pressure has changed since the last call!
        b.setAirPressureCallbackPeriod(1000);
    }
);

// Register air pressure callback
b.on(Tinkerforge.BrickletBarometer.CALLBACK_AIR_PRESSURE,
    // Callback function for air pressure callback
    function (airPressure) {
        console.log('Air Pressure: ' + airPressure/1000.0 + ' mbar');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
