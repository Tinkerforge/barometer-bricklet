Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "SCB31" ' Change to your UID

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current air pressure (unit is mbar/1000)
        Dim airPressure As Integer = b.GetAirPressure()

        ' Get current altitude (unit is mbar/1000)
        Dim altitude As Integer = b.GetAltitude()

        System.Console.WriteLine("Air Pressure: " + (airPressure/1000.0).ToString() + " mbar")
        System.Console.WriteLine("Altitude: " + (altitude/100.0).ToString() + " m")

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
