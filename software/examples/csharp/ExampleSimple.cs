using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current air pressure (unit is mbar/1000)
		int airPressure = b.GetAirPressure();
		System.Console.WriteLine("Air Pressure: " + airPressure/1000.0 + " mbar");

		// Get current altitude (unit is cm)
		int altitude = b.GetAltitude();
		System.Console.WriteLine("Altitude: " + altitude/100.0 + " m");

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
