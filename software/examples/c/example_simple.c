#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	Barometer b;
	barometer_create(&b, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Get current air pressure (unit is mbar/1000)
	int32_t air_pressure;
	if(barometer_get_air_pressure(&b, &air_pressure) < 0) {
		fprintf(stderr, "Could not get air pressure, probably timeout\n");
		return 1;
	}

	printf("Air Pressure: %f mbar\n", air_pressure/1000.0);

	// Get current altitude (unit is cm)
	int32_t altitude;
	if(barometer_get_altitude(&b, &altitude) < 0) {
		fprintf(stderr, "Could not get altitude, probably timeout\n");
		return 1;
	}

	printf("Altitude: %f m\n", altitude/100.0);

	printf("Press key to exit\n");
	getchar();
	barometer_destroy(&b);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
