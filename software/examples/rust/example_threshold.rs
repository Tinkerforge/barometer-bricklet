use std::{error::Error, io, thread};
use tinkerforge::{barometer_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Barometer Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let barometer_bricklet = BarometerBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get threshold listeners with a debounce time of 10 seconds (10000ms)
    barometer_bricklet.set_debounce_period(10000);

    //Create listener for air pressure reached events.
    let air_pressure_reached_listener = barometer_bricklet.get_air_pressure_reached_receiver();
    // Spawn thread to handle received events. This thread ends when the barometer_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in air_pressure_reached_listener {
            println!("Air Pressure: {}{}", event as f32 / 1000.0, " mbar");
            println!("Enjoy the potentially good weather!");
        }
    });

    // Configure threshold for air pressure "greater than 1025 mbar"
    barometer_bricklet.set_air_pressure_callback_threshold('>', 1025 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
