Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "SCB31" ' Change to your UID

    ' Callback function for air pressure callback (parameter has unit mbar/1000)
    Sub AirPressureCB(ByVal sender As BrickletBarometer, ByVal airPressure As Integer)
        System.Console.WriteLine("Air Pressure: " + (airPressure/1000.0).ToString() + " mbar")
    End Sub

    ' Callback function for altitude callback (parameter has unit cm)
    Sub AltitudeCB(ByVal sender As BrickletBarometer, ByVal altitude As Integer)
        System.Console.WriteLine("Altitude: " + (altitude/100.0).ToString() + " m")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set Period for air pressure and altitude callbacks to 1s (1000ms)
        ' Note: The air pressure and altitude callbacks are only called every second
        '       if the air pressure or altitude has changed since the last call!
        b.SetAirPressureCallbackPeriod(1000)
        b.SetAltitudeCallbackPeriod(1000)

        ' Register air pressure callback to function AirPressureCB
        AddHandler b.AirPressure, AddressOf AirPressureCB

        ' Register altitude callback to function AltitudeCB
        AddHandler b.Altitude, AddressOf AltitudeCB

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
