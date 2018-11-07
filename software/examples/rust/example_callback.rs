use std::{error::Error, io, thread};
use tinkerforge::{barometer_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let b = BarometerBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let air_pressure_receiver = b.get_air_pressure_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `b` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for air_pressure in air_pressure_receiver {
            println!("Air Pressure: {} mbar", air_pressure as f32 / 1000.0);
        }
    });

    // Set period for air pressure receiver to 1s (1000ms).
    // Note: The air pressure callback is only called every second
    //       if the air pressure has changed since the last call!
    b.set_air_pressure_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
