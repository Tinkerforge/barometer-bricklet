function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletBarometer;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Barometer Bricklet

    ipcon = IPConnection(); % Create IP connection
    b = handle(BrickletBarometer(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current air pressure (unit is mbar/1000)
    airPressure = b.getAirPressure();
    fprintf('Air Pressure: %g mbar\n', airPressure/1000.0);

    % Get current altitude (unit is cm)
    altitude = b.getAltitude();
    fprintf('Altitude: %g m\n', altitude/100.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
