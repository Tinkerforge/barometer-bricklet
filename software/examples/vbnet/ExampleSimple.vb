Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Barometer Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current air pressure
        Dim airPressure As Integer = b.GetAirPressure()
        Console.WriteLine("Air Pressure: " + (airPressure/1000.0).ToString() + " hPa")

        ' Get current altitude
        Dim altitude As Integer = b.GetAltitude()
        Console.WriteLine("Altitude: " + (altitude/100.0).ToString() + " m")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
