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

    // Get threshold receivers with a debounce time of 10 seconds (10000ms).
    b.set_debounce_period(10000);

    let air_pressure_reached_receiver = b.get_air_pressure_reached_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `b` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for air_pressure_reached in air_pressure_reached_receiver {
            println!("Air Pressure: {} hPa", air_pressure_reached as f32 / 1000.0);
            println!("Enjoy the potentially good weather!");
        }
    });

    // Configure threshold for air pressure "greater than 1025 hPa".
    b.set_air_pressure_callback_threshold('>', 1025 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
