package main

import (
	"fmt"
	"tinkerforge/barometer_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Barometer Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	b, _ := barometer_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get threshold receivers with a debounce time of 10 seconds (10000ms).
	b.SetDebouncePeriod(10000)

	b.RegisterAirPressureReachedCallback(func(airPressure int32) {
		fmt.Printf("Air Pressure: %d mbar\n", float64(airPressure)/1000.0)
		fmt.Println("Enjoy the potentially good weather!")
	})

	// Configure threshold for air pressure "greater than 1025 mbar".
	b.SetAirPressureCallbackThreshold('>', 1025*1000, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
