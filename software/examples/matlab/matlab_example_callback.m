function matlab_example_callback
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletBarometer;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'jmQ'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    b = BrickletBarometer(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set Period for air pressure and altitude callbacks to 1s (1000ms)
    % Note: The air pressure and altitude callbacks are only called every second
    %       if the air pressure or altitude has changed since the last call!
    b.setAirPressureCallbackPeriod(1000);
    b.setAltitudeCallbackPeriod(1000);

    % Register air pressure callback to function cb_air_pressure
    set(b, 'AirPressureCallback', @(h, e)cb_air_pressure(e.airPressure));

    % Register altitude callback to function cb_altitude
    set(b, 'AltitudeCallback', @(h, e)cb_altitude(e.altitude));

    input('\nPress any key to exit...\n', 's');
    ipcon.disconnect();
end

%Callback function for air pressure callback (parameter has unit mbar/1000)
function cb_air_pressure(air_pressure)
    fprintf('Air Pressure: %gmbar\n', air_pressure/1000);
end

% Callback function for altitude callback (parameter has unit cm)
function cb_altitude(altitude)
    fprintf('Altitude: %gm\n', altitude/100);
end
