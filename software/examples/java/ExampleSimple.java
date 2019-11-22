import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletBarometer;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Barometer Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current air pressure
		int airPressure = b.getAirPressure(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Air Pressure: " + airPressure/1000.0 + " hPa");

		// Get current altitude
		int altitude = b.getAltitude(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Altitude: " + altitude/100.0 + " m");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
