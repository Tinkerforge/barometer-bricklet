Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Barometer Bricklet

    ' Callback subroutine for air pressure reached callback (parameter has unit mbar/1000)
    Sub AirPressureReachedCB(ByVal sender As BrickletBarometer, _
                             ByVal airPressure As Integer)
        Console.WriteLine("Air Pressure: " + (airPressure/1000.0).ToString() + " mbar")
        Console.WriteLine("Enjoy the potentially good weather!")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        b.SetDebouncePeriod(10000)

        ' Register air pressure reached callback to subroutine AirPressureReachedCB
        AddHandler b.AirPressureReachedCallback, AddressOf AirPressureReachedCB

        ' Configure threshold for air_pressure "greater than 1025 mbar" (unit is mbar/1000)
        b.SetAirPressureCallbackThreshold(">"C, 1025*1000, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
