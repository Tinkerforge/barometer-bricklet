import com.tinkerforge.BrickletBarometer;
import com.tinkerforge.IPConnection;

public class ExampleThreshold {
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

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		b.setDebouncePeriod(10000);

		// Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
		b.setAirPressureCallbackThreshold('>', 1025*1000, 0);

		// Add and implement air pressure reached listener
		// (called if air pressure is greater than 1025 mbar)
		b.addListener(new BrickletBarometer.AirPressureReachedListener() {
			public void airPressureReached(int airPressure) {
				System.out.println("We have: " + airPressure/1000.0 + " mbar.");
				System.out.println("Enjoy the potentially good weather!");
			}
		});

		System.console().readLine("Press key to exit\n");
		ipcon.destroy();
	}
}
