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
        // Get current air pressure
        b.getAirPressure(
            function (airPressure) {
                console.log('Air Pressure: ' + airPressure/1000.0 + ' mbar');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );

        // Get current altitude
        b.getAltitude(
            function (altitude) {
                console.log('Altitude: ' + altitude/100.0 + ' m');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
