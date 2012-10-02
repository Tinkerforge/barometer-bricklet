using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "bAc"; // Change to your UID

	static void Main()
	{
		IPConnection ipcon = new IPConnection(HOST, PORT); // Create connection to brickd
		BrickletBarometer b = new BrickletBarometer(UID); // Create device object
		ipcon.AddDevice(b); // Add device to IP connection
		// Don't use device before it is added to a connection

		// Get current air pressure (unit is mbar/1000)
		int airPressure = b.GetAirPressure();

		// Get current altitude (unit is mbar/1000)
		int altitude = b.GetAltitude();

		System.Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");
		System.Console.WriteLine("Altitude: " + altitude/100.0 + " m");

		System.Console.WriteLine("Press key to exit");
		System.Console.ReadKey();
		ipcon.Destroy();
	}
}
