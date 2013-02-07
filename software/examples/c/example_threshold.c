
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "bAc" // Change to your UID

// Callback for air pressure greater than 1025 mbar
void cb_reached(int32_t air_pressure, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("We have %f mbar.\n", air_pressure/1000.0);
	printf("Enjoy the potentially good weather!\n");
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

	// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
	barometer_set_debounce_period(&b, 10000);

	// Register threshold reached callback to function cb_reached
	barometer_register_callback(&b,
	                            BAROMETER_CALLBACK_AIR_PRESSURE_REACHED,
	                            (void *)cb_reached,
	                            NULL);

	// Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
	barometer_set_air_pressure_callback_threshold(&b, '>', 1025*1000, 0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
