
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "bAc" // Change to your UID

// Callback function for air pressure callback (parameter has unit mbar/1000)
void cb_air_pressure(int32_t air_pressure) {
	printf("Air Pressure: %f mbar\n", air_pressure/1000.0);
}

// Callback function for altitude callback (parameter has unit cm)
void cb_altitude(int32_t altitude) {
	printf("Altitude: %f m\n", altitude/100.0);
}

int main() {
	// Create IP connection to brickd
	IPConnection ipcon;
	if(ipcon_create(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not create connection\n");
		exit(1);
	}

	// Create device object
	Barometer b;
	barometer_create(&b, UID);

	// Add device to IP connection
	if(ipcon_add_device(&ipcon, &b) < 0) {
		fprintf(stderr, "Could not connect to Bricklet\n");
		exit(1);
	}
	// Don't use device before it is added to a connection

	// Set Period for air pressure and altitude callbacks to 1s (1000ms)
	// Note: The air pressure and altitude callbacks are only called every second
	//       if the air pressure or altitude has changed since the last call!
	barometer_set_air_pressure_callback_period(&b, 1000);
	barometer_set_altitude_callback_period(&b, 1000);

	// Register air pressure callback to function cb_air_pressure
	barometer_register_callback(&b,
	                            BAROMETER_CALLBACK_AIR_PRESSURE,
	                            cb_air_pressure);

	// Register altitude callback to function cb_altitude
	barometer_register_callback(&b,
	                            BAROMETER_CALLBACK_ALTITUDE,
	                            cb_altitude);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon);
}
