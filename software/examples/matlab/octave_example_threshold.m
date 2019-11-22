function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Barometer Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    b = javaObject("com.tinkerforge.BrickletBarometer", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    b.setDebouncePeriod(10000);

    % Register air pressure reached callback to function cb_air_pressure_reached
    b.addAirPressureReachedCallback(@cb_air_pressure_reached);

    % Configure threshold for air pressure "greater than 1025 hPa"
    b.setAirPressureCallbackThreshold(">", 1025*1000, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for air pressure reached callback
function cb_air_pressure_reached(e)
    fprintf("Air Pressure: %g hPa\n", e.airPressure/1000.0);
    fprintf("Enjoy the potentially good weather!\n");
end
