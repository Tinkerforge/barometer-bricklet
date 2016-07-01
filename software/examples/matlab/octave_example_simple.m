function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Barometer Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    b = java_new("com.tinkerforge.BrickletBarometer", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current air pressure (unit is mbar/1000)
    airPressure = b.getAirPressure();
    fprintf("Air Pressure: %g mbar\n", airPressure/1000.0);

    % Get current altitude (unit is cm)
    altitude = b.getAltitude();
    fprintf("Altitude: %g m\n", altitude/100.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
