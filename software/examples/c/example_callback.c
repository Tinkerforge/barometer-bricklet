
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "bAc" // Change to your UID

// Callback function for air pressure callback (parameter has unit mbar/1000)
void cb_air_pressure(int32_t air_pressure, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Air Pressure: %f mbar\n", air_pressure/1000.0);
}

// Callback function for altitude callback (parameter has unit cm)
void cb_altitude(int32_t altitude, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Altitude: %f m\n", altitude/100.0);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	Barometer b;
	barometer_create(&b, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Set Period for air pressure and altitude callbacks to 1s (1000ms)
	// Note: The air pressure and altitude callbacks are only called every second
	//       if the air pressure or altitude has changed since the last call!
	barometer_set_air_pressure_callback_period(&b, 1000);
	barometer_set_altitude_callback_period(&b, 1000);

	// Register air pressure callback to function cb_air_pressure
	barometer_register_callback(&b,
	                            BAROMETER_CALLBACK_AIR_PRESSURE,
	                            (void *)cb_air_pressure,
	                            NULL);

	// Register altitude callback to function cb_altitude
	barometer_register_callback(&b,
	                            BAROMETER_CALLBACK_ALTITUDE,
	                            (void *)cb_altitude,
	                            NULL);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
