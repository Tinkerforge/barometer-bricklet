function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Barometer Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    b = java_new("com.tinkerforge.BrickletBarometer", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register air pressure callback to function cb_air_pressure
    b.addAirPressureCallback(@cb_air_pressure);

    % Set period for air pressure callback to 1s (1000ms)
    % Note: The air pressure callback is only called every second
    %       if the air pressure has changed since the last call!
    b.setAirPressureCallbackPeriod(1000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for air pressure callback (parameter has unit mbar/1000)
function cb_air_pressure(e)
    fprintf("Air Pressure: %g mbar\n", e.airPressure/1000.0);
end
