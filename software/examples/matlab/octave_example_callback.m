function octave_example_callback()
    more off;
    
    HOST = "localhost";
    PORT = 4223;
    UID ="jmQ"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    b = java_new("com.tinkerforge.BrickletBarometer", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected
    
    % Set Period for air pressure and altitude callbacks to 1s (1000ms)
    % Note: The air pressure and altitude callbacks are only called every second
    %       if the air pressure or altitude has changed since the last call!
    b.setAirPressureCallbackPeriod(1000);
    b.setAltitudeCallbackPeriod(1000);

    % Register air pressure callback to function cb_air_pressure
    b.addAirPressureCallback(@cb_air_pressure);

    % Register altitude callback to function cb_altitude
    b.addAltitudeCallback(@cb_altitude);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for air pressure callback (parameter has unit mbar/1000)
function cb_air_pressure(e)
    fprintf("Air Pressure: %g mbar\n", e.airPressure/1000.0);
end

% Callback function for altitude callback (parameter has unit cm)
function cb_altitude(e)
    fprintf("Altitude: %g m\n", e.altitude/100.0);
end
