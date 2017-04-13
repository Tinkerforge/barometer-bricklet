using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet

	// Callback function for air pressure reached callback (parameter has unit mbar/1000)
	static void AirPressureReachedCB(BrickletBarometer sender, int airPressure)
	{
		Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");
		Console.WriteLine("Enjoy the potentially good weather!");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		b.SetDebouncePeriod(10000);

		// Register air pressure reached callback to function AirPressureReachedCB
		b.AirPressureReachedCallback += AirPressureReachedCB;

		// Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
		b.SetAirPressureCallbackThreshold('>', 1025*1000, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
