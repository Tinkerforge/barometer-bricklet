
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "bAc" // Change to your UID

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

	// Get current air pressure (unit is mbar/1000)
	int32_t air_pressure;
	if(barometer_get_air_pressure(&b, &air_pressure) < 0) {
		fprintf(stderr, "Could not get air pressure, probably timeout\n");
		exit(1);
	}

	printf("Air Pressure: %f mbar\n", air_pressure/1000.0);

	// Get current altitude (unit is cm)
	int32_t altitude;
	if(barometer_get_altitude(&b, &altitude) < 0) {
		fprintf(stderr, "Could not get altitude, probably timeout\n");
		exit(1);
	}

	printf("Altitude: %f m\n", altitude/100.0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon);
}
