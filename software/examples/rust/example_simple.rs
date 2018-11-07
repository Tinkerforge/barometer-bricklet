use std::{error::Error, io};

use tinkerforge::{barometer_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let b = BarometerBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current air pressure.
    let air_pressure = b.get_air_pressure().recv()?;
    println!("Air Pressure: {} mbar", air_pressure as f32 / 1000.0);

    // Get current altitude.
    let altitude = b.get_altitude().recv()?;
    println!("Altitude: {} m", altitude as f32 / 100.0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
