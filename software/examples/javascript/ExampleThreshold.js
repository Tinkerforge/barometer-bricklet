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
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        b.setDebouncePeriod(10000);
        // Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
        b.setAirPressureCallbackThreshold('>', 1025*1000, 0);    
    }
);

// Register threshold reached callback
b.on(Tinkerforge.BrickletBarometer.CALLBACK_AIR_PRESSURE_REACHED,
    // Callback for air pressure greater than 1025 mbar
    function(ap) {
        console.log('We have '+ap/1000.+' mbar.');
        console.log('Enjoy the potentially good weather!');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

