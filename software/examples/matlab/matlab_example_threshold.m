function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletBarometer;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Barometer Bricklet

    ipcon = IPConnection(); % Create IP connection
    b = handle(BrickletBarometer(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    b.setDebouncePeriod(10000);

    % Register air pressure reached callback to function cb_air_pressure_reached
    set(b, 'AirPressureReachedCallback', @(h, e) cb_air_pressure_reached(e));

    % Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
    b.setAirPressureCallbackThreshold('>', 1025*1000, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for air pressure reached callback (parameter has unit mbar/1000)
function cb_air_pressure_reached(e)
    fprintf('Air Pressure: %g mbar\n', e.airPressure/1000.0);
    fprintf('Enjoy the potentially good weather!\n');
end
