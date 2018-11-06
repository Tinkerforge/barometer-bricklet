use std::{error::Error, io};

use tinkerforge::{barometer_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let barometer_bricklet = BarometerBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current air pressure
    let air_pressure = barometer_bricklet.get_air_pressure().recv()?;
    println!("Air Pressure: {}{}", air_pressure as f32 / 1000.0, " mbar");

    // Get current altitude
    let altitude = barometer_bricklet.get_altitude().recv()?;
    println!("Altitude: {}{}", altitude as f32 / 100.0, " m");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
