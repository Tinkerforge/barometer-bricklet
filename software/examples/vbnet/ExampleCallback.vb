Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Barometer Bricklet

    ' Callback subroutine for air pressure callback (parameter has unit mbar/1000)
    Sub AirPressureCB(ByVal sender As BrickletBarometer, ByVal airPressure As Integer)
        Console.WriteLine("Air Pressure: " + (airPressure/1000.0).ToString() + " mbar")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register air pressure callback to subroutine AirPressureCB
        AddHandler b.AirPressure, AddressOf AirPressureCB

        ' Set period for air pressure callback to 1s (1000ms)
        ' Note: The air pressure callback is only called every second
        '       if the air pressure has changed since the last call!
        b.SetAirPressureCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
