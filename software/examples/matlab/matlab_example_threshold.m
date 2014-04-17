function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletBarometer;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'jmQ'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    b = BrickletBarometer(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    b.setDebouncePeriod(10000);

    % Register threshold reached callback to function cb_reached
    set(b, 'AirPressureReachedCallback', @(h, e) cb_reached(e));

    % Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
    b.setAirPressureCallbackThreshold('>', 1025*1000, 0);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback for air pressure greater than 1025 mbar
function cb_reached(e)
    fprintf('We have %g mbar\n', e.airPressure/1000.0);
    fprintf('Enjoy the potentially good weather!\n');
end
