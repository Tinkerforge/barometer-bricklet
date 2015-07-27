using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for air pressure callback (parameter has unit mbar/1000)
	static void AirPressureCB(BrickletBarometer sender, int airPressure)
	{
		System.Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");
	}

	// Callback function for altitude callback (parameter has unit cm)
	static void AltitudeCB(BrickletBarometer sender, int altitude)
	{
		System.Console.WriteLine("Altitude: " + altitude/100.0 + " m");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set period for air pressure callback to 1s (1000ms)
		// Note: The air pressure callback is only called every second
		//       if the air pressure has changed since the last call!
		b.SetAirPressureCallbackPeriod(1000);

		// Register air pressure callback to function AirPressureCB
		b.AirPressure += AirPressureCB;

		// Set period for altitude callback to 1s (1000ms)
		// Note: The altitude callback is only called every second
		//       if the altitude has changed since the last call!
		b.SetAltitudeCallbackPeriod(1000);

		// Register altitude callback to function AltitudeCB
		b.Altitude += AltitudeCB;

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
