using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "bAc"; // Change to your UID

	// Callback for air pressure greater than 1025 mbar
	static void ReachedCB(object sender, int airPressure)
	{
		System.Console.WriteLine("We have " + airPressure/1000.0 + " mbar.");
		System.Console.WriteLine("Enjoy the potentially good weather!");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		b.SetDebouncePeriod(10000);

		// Register threshold reached callback to function ReachedCB
		b.AirPressureReached += ReachedCB;

		// Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
		b.SetAirPressureCallbackThreshold('>', 1025*1000, 0);

		System.Console.WriteLine("Press key to exit");
		System.Console.ReadKey();
	}
}
