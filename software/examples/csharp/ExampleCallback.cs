using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet

	// Callback function for air pressure callback (parameter has unit mbar/1000)
	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register air pressure callback to function AirPressureCB
		b.AirPressure += AirPressureCB;

		// Set period for air pressure callback to 1s (1000ms)
		// Note: The air pressure callback is only called every second
		//       if the air pressure has changed since the last call!
		b.SetAirPressureCallbackPeriod(1000);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
