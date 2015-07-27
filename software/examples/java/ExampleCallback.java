import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletBarometer;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletBarometer b = new BrickletBarometer(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set period for air pressure callback to 1s (1000ms)
		// Note: The air pressure callback is only called every second
		//       if the air pressure has changed since the last call!
		b.setAirPressureCallbackPeriod(1000);

		// Add air pressure listener (parameter has unit mbar/1000)
		b.addAirPressureListener(new BrickletBarometer.AirPressureListener() {
			public void airPressure(int airPressure) {
				System.out.println("Air Pressure: " + airPressure/1000.0 + " mbar");
			}
		});

		// Set period for altitude callback to 1s (1000ms)
		// Note: The altitude callback is only called every second
		//       if the altitude has changed since the last call!
		b.setAltitudeCallbackPeriod(1000);

		// Add altitude listener (parameter has unit cm)
		b.addAltitudeListener(new BrickletBarometer.AltitudeListener() {
			public void altitude(int altitude) {
				System.out.println("Altitude: " + altitude/100.0 + " m");
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
