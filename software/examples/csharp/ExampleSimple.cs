using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current air pressure
		int airPressure = b.GetAirPressure();
		Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");

		// Get current altitude
		int altitude = b.GetAltitude();
		Console.WriteLine("Altitude: " + altitude/100.0 + " m");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
