import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletBarometer;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		b.setDebouncePeriod(10000);

		// Add air pressure reached listener (parameter has unit mbar/1000)
		b.addAirPressureReachedListener(new BrickletBarometer.AirPressureReachedListener() {
			public void airPressureReached(int airPressure) {
				System.out.println("Air Pressure: " + airPressure/1000.0 + " mbar");
				System.out.println("Enjoy the potentially good weather!");
			}
		});

		// Configure threshold for air pressure "greater than 1025 mbar" (unit is mbar/1000)
		b.setAirPressureCallbackThreshold('>', 1025*1000, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
