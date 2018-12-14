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

	b.RegisterAirPressureCallback(func(airPressure int32) {
		fmt.Printf("Air Pressure: %f mbar\n", float64(airPressure)/1000.0)
	})

	// Set period for air pressure receiver to 1s (1000ms).
	// Note: The air pressure callback is only called every second
	//       if the air pressure has changed since the last call!
	b.SetAirPressureCallbackPeriod(1000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
