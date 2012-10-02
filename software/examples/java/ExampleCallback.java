import com.tinkerforge.BrickletBarometer;
import com.tinkerforge.IPConnection;

public class ExampleCallback {
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

		// Set Period for air pressure and altitude callbacks to 1s (1000ms)
		// Note: The air pressure and altitude callbacks are only called every second
		//       if the air pressure or altitude has changed since the last call!
		b.setAirPressureCallbackPeriod(1000);
		b.setAltitudeCallbackPeriod(1000);

		// Add and implement air pressure listener (called if air pressure changes)
		b.addListener(new BrickletBarometer.AirPressureListener() {
			public void airPressure(int airPressure) {
				System.out.println("Air Pressure: " + airPressure/1000.0 + " mbar");
			}
		});

		// Add and implement altitude listener (called if altitude changes)
		b.addListener(new BrickletBarometer.AltitudeListener() {
			public void altitude(int altitude) {
				System.out.println("Altitude: " + altitude/100.0 + " m");
			}
		});

		System.console().readLine("Press key to exit\n");
		ipcon.destroy();
	}
}
