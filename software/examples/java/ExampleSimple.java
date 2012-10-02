import com.tinkerforge.BrickletBarometer;
import com.tinkerforge.IPConnection;

public class ExampleSimple {
	private static final String host = "localhost";
	private static final int port = 4223;
	private static final String UID = "bAc"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the commnents below
	public static void main(String args[]) throws Exception {
		// Create connection to brickd
		IPConnection ipcon = new IPConnection(host, port); // Can throw IOException
		BrickletBarometer b = new BrickletBarometer(UID); // Create device object

		// Add device to IP connection
		ipcon.addDevice(b); // Can throw IPConnection.TimeoutException
		// Don't use device before it is added to a connection

		// Get current air pressure (unit is mbar/1000)
		int airPressure = b.getAirPressure(); // Can throw IPConnection.TimeoutException

		System.out.println("Air Pressure: " + airPressure/1000.0 + " Lux");

		// Get current altitude (unit is cm)
		int altitude = b.getAltitude(); // Can throw IPConnection.TimeoutException

		System.out.println("Altitude: " + altitude/100.0 + " m");

		System.console().readLine("Press key to exit\n");
		ipcon.destroy();
	}
}
