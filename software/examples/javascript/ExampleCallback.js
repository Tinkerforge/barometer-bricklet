var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'jmQ';// Change to your UID

var ipcon = new Tinkerforge.IPConnection();// Create IP connection
var b = new Tinkerforge.BrickletBarometer(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Set Period for air pressure and altitude callbacks to 1s (1000ms)
        // Note: The air pressure and altitude callbacks are only called every second
        // if the air pressure or altitude has changed since the last call!
        b.setAirPressureCallbackPeriod(1000);
        b.setAltitudeCallbackPeriod(1000);   
    }
);

// Register air pressure callback
b.on(Tinkerforge.BrickletBarometer.CALLBACK_AIR_PRESSURE,
    // Callback function for air pressure callback (parameter has unit mbar/1000)
    function(ap) {
        console.log('Air pressure: '+ap/1000+' mbar');
        console.log();
    }
);

// Register altitude callback
b.on(Tinkerforge.BrickletBarometer.CALLBACK_ALTITUDE,
    // Callback function for altitude callback (parameter has unit cm)
    function(alt) {
        console.log('Altitude: '+alt/100+' m');
        console.log();
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

