var IPConnection = require('Tinkerforge/IPConnection');
var BrickletBarometer = require('Tinkerforge/BrickletBarometer');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'jmQ';// Change to your UID

var ipcon = new IPConnection();// Create IP connection
var b = new BrickletBarometer(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Get current air pressure (unit is mbar/1000) and current altitude (unit is cm)
        b.getAirPressure(
            function(ap) {
                console.log('Air pressure: '+ap/1000+' mbar');
            },
            function(error) {
                console.log('Error: '+error);
            }
        );
        b.getAltitude(
            function(alt) {
                console.log('Altitude: '+alt/100+' m');
            },
            function(error) {
                console.log('Error: '+error);
            }
        );
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

