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

    //Create listener for air pressure events.
    let air_pressure_listener = barometer_bricklet.get_air_pressure_receiver();
    // Spawn thread to handle received events. This thread ends when the barometer_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in air_pressure_listener {
            println!("Air Pressure: {}{}", event as f32 / 1000.0, " mbar");
        }
    });

    // Set period for air pressure listener to 1s (1000ms)
    // Note: The air pressure callback is only called every second
    //       if the air pressure has changed since the last call!
    barometer_bricklet.set_air_pressure_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
