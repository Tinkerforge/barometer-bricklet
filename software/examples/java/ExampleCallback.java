import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletBarometer;

public class ExampleCallback {
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

		// Add air pressure listener
		b.addAirPressureListener(new BrickletBarometer.AirPressureListener() {
			public void airPressure(int airPressure) {
				System.out.println("Air Pressure: " + airPressure/1000.0 + " hPa");
			}
		});

		// Set period for air pressure callback to 1s (1000ms)
		// Note: The air pressure callback is only called every second
		//       if the air pressure has changed since the last call!
		b.setAirPressureCallbackPeriod(1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
