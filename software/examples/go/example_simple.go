package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/barometer_bricklet"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
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

	// Get current air pressure.
	airPressure, _ := b.GetAirPressure()
	fmt.Printf("Air Pressure: %f mbar\n", float64(airPressure)/1000.0)

	// Get current altitude.
	altitude, _ := b.GetAltitude()
	fmt.Printf("Altitude: %f m\n", float64(altitude)/100.0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
